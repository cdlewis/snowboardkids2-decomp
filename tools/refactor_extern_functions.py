#!/usr/bin/env python3
"""Move project function declarations out of C files and into owner headers.

For every top-level ``extern`` function declaration in ``src/**/*.c`` whose
definition can be found in exactly one project C file, this script:

* declares the function in ``include/<owner path>.h``;
* replaces the local declaration with an include of that header; and
* creates the owner header when it does not exist.

The default mode is a dry run. Use ``--apply`` to write changes or ``--check``
to make remaining refactors fail with a non-zero exit status.

This intentionally does not process extern variables, function-pointer
objects, static functions, or symbols whose owner is missing or ambiguous.
It is a lightweight source scanner rather than a full C parser, but it tracks
comments, strings, preprocessor directives, parentheses, and brace depth so
multiline declarations and definitions are handled safely.
"""

from __future__ import annotations

import argparse
import difflib
import re
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


IDENTIFIER_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
INCLUDE_RE = re.compile(
    r'^[ \t]*#[ \t]*include[ \t]*[<"]([^>"]+)[>"][^\n]*(?:\n|$)',
    re.MULTILINE,
)
NON_FUNCTION_NAMES = {
    "__attribute__",
    "__declspec",
    "alignof",
    "for",
    "if",
    "sizeof",
    "switch",
    "typeof",
    "while",
}


@dataclass(frozen=True)
class FunctionDefinition:
    name: str
    path: Path
    line: int


@dataclass(frozen=True)
class FunctionDeclaration:
    name: str
    path: Path
    line: int
    start: int
    end: int
    text: str


@dataclass(frozen=True)
class Diagnostic:
    path: Path
    line: int
    message: str


def mask_non_code(text: str) -> str:
    """Replace comments, literals, and preprocessor directives with spaces."""
    chars = list(text)
    result = list(text)
    state = "code"
    line_has_code = False
    index = 0

    def blank(position: int) -> None:
        if result[position] != "\n":
            result[position] = " "

    while index < len(chars):
        char = chars[index]
        following = chars[index + 1] if index + 1 < len(chars) else ""

        if state == "code":
            if char == "\n":
                line_has_code = False
            elif not line_has_code and char in " \t\r":
                pass
            elif not line_has_code and char == "#":
                blank(index)
                state = "preprocessor"
            elif char == "/" and following == "/":
                blank(index)
                blank(index + 1)
                index += 1
                state = "line_comment"
            elif char == "/" and following == "*":
                blank(index)
                blank(index + 1)
                index += 1
                state = "block_comment"
            elif char == '"':
                blank(index)
                state = "string"
                line_has_code = True
            elif char == "'":
                blank(index)
                state = "character"
                line_has_code = True
            elif char not in " \t\r":
                line_has_code = True

        elif state == "line_comment":
            blank(index)
            if char == "\n":
                state = "code"
                line_has_code = False

        elif state == "block_comment":
            blank(index)
            if char == "*" and following == "/":
                blank(index + 1)
                index += 1
                state = "code"

        elif state in {"string", "character"}:
            blank(index)
            quote = '"' if state == "string" else "'"
            if char == "\\" and following:
                blank(index + 1)
                index += 1
            elif char == quote:
                state = "code"
            elif char == "\n":
                state = "code"
                line_has_code = False

        elif state == "preprocessor":
            blank(index)
            if char == "\n":
                previous = index - 1
                while previous >= 0 and chars[previous] in " \t\r":
                    previous -= 1
                if previous < 0 or chars[previous] != "\\":
                    state = "code"
                    line_has_code = False

        index += 1

    return "".join(result)


def first_top_level_open_paren(text: str) -> int | None:
    depth = 0
    for index, char in enumerate(text):
        if char == "(":
            if depth == 0:
                return index
            depth += 1
        elif char == ")" and depth:
            depth -= 1
    return None


def function_name(declaration: str) -> str | None:
    """Return a direct function declarator's name, excluding pointer objects."""
    open_paren = first_top_level_open_paren(declaration)
    if open_paren is None:
        return None

    prefix = declaration[:open_paren].rstrip()
    matches = list(IDENTIFIER_RE.finditer(prefix))
    if not matches:
        return None
    match = matches[-1]
    name = match.group(0)
    if name in NON_FUNCTION_NAMES:
        return None

    # In ``void (*callback)(void)``, the first top-level parenthesis belongs to
    # a parenthesized pointer declarator, not a function declaration.
    if prefix[match.end() :].strip() or prefix.endswith("*"):
        return None
    return name


def iter_top_level_items(text: str) -> Iterable[tuple[str, int, int]]:
    """Yield top-level statements and function/initializer prefixes."""
    masked = mask_non_code(text)
    brace_depth = 0
    start = 0

    for index, char in enumerate(masked):
        if char == "{":
            if brace_depth == 0:
                yield "brace", start, index
            brace_depth += 1
        elif char == "}":
            if brace_depth:
                brace_depth -= 1
                if brace_depth == 0:
                    start = index + 1
        elif char == ";" and brace_depth == 0:
            yield "statement", start, index + 1
            start = index + 1


def scan_c_file(path: Path) -> tuple[list[FunctionDefinition], list[FunctionDeclaration]]:
    text = path.read_text(encoding="utf-8")
    masked = mask_non_code(text)
    definitions: list[FunctionDefinition] = []
    declarations: list[FunctionDeclaration] = []

    for kind, start, end in iter_top_level_items(text):
        code = masked[start:end].strip()
        if not code:
            continue

        if kind == "brace":
            if code.startswith(("static ", "typedef ", "extern ")):
                continue
            if "=" in code:
                continue
            name = function_name(code)
            if name:
                definitions.append(
                    FunctionDefinition(name, path, text.count("\n", 0, start) + 1)
                )
            continue

        if not re.match(r"^extern(?:\s|$)", code):
            continue
        name = function_name(code[:-1].rstrip())
        if not name:
            continue

        extern_match = re.search(r"\bextern\b", masked[start:end])
        if extern_match is None:
            continue
        declaration_start = start + extern_match.start()
        remove_start, remove_end = whole_line_span(text, declaration_start, end)
        declarations.append(
            FunctionDeclaration(
                name=name,
                path=path,
                line=text.count("\n", 0, declaration_start) + 1,
                start=remove_start,
                end=remove_end,
                text=text[declaration_start:end].strip(),
            )
        )

    return definitions, declarations


def whole_line_span(text: str, start: int, end: int) -> tuple[int, int]:
    """Expand a declaration to whole lines when surrounding text is blank."""
    line_start = text.rfind("\n", 0, start) + 1
    next_newline = text.find("\n", end)
    line_end = len(text) if next_newline < 0 else next_newline + 1
    if not text[line_start:start].strip() and not text[end:line_end].strip():
        return line_start, line_end
    return start, end


def corresponding_header(root: Path, source: Path) -> Path:
    relative = source.relative_to(root / "src").with_suffix(".h")
    return root / "include" / relative


def include_name(root: Path, header: Path) -> str:
    return header.relative_to(root / "include").as_posix()


def declaration_without_extern(text: str) -> str:
    declaration = re.sub(r"^extern\s+", "", text.strip(), count=1)
    return declaration + ("" if declaration.endswith(";") else ";")


def declared_function_names(text: str) -> set[str]:
    masked = mask_non_code(text)
    names: set[str] = set()
    for kind, start, end in iter_top_level_items(text):
        if kind != "statement":
            continue
        code = masked[start:end].strip()
        if not code or code.startswith("typedef "):
            continue
        name = function_name(code[:-1].rstrip())
        if name:
            names.add(name)
    return names


def normalized_declaration(text: str) -> str:
    text = declaration_without_extern(text)
    return re.sub(r"\s+", " ", mask_non_code(text)).strip()


def add_include(text: str, name: str) -> str:
    if name in {match.group(1) for match in INCLUDE_RE.finditer(text)}:
        return text

    directive = f'#include "{name}"\n'
    includes = list(INCLUDE_RE.finditer(text))
    if not includes:
        return directive + ("\n" if text and not text.startswith("\n") else "") + text

    quoted = [match for match in includes if '"' in match.group(0)]
    for match in quoted:
        if match.group(1) > name:
            return text[: match.start()] + directive + text[match.start() :]
    anchor = quoted[-1] if quoted else includes[-1]
    return text[: anchor.end()] + directive + text[anchor.end() :]


def add_header_declarations(text: str, declarations: list[str]) -> str:
    block = "\n".join(declarations) + "\n"
    endif_matches = list(re.finditer(r"^[ \t]*#endif\b[^\n]*(?:\n|$)", text, re.MULTILINE))
    if endif_matches:
        position = endif_matches[-1].start()
        prefix = text[:position].rstrip() + "\n\n"
        return prefix + block + text[position:]
    return text.rstrip() + "\n\n" + block


def new_header(declarations: list[str]) -> str:
    return (
        '#pragma once\n\n#include "common.h"\n\n'
        + "\n".join(declarations)
        + "\n"
    )


def apply_replacements(text: str, replacements: list[tuple[int, int, str]]) -> str:
    for start, end, replacement in sorted(replacements, reverse=True):
        text = text[:start] + replacement + text[end:]
    return text


def relative(path: Path, root: Path) -> str:
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return str(path)


def project_source_files(root: Path) -> list[Path]:
    """Return tracked source files, falling back to all sources outside Git."""
    try:
        result = subprocess.run(
            ["git", "ls-files", "-z", "--", "src/**/*.c", "src/*.c"],
            cwd=root,
            check=True,
            capture_output=True,
            text=True,
        )
    except (FileNotFoundError, subprocess.CalledProcessError):
        return sorted((root / "src").rglob("*.c"))

    tracked = [root / item for item in result.stdout.split("\0") if item]
    return sorted(path for path in tracked if path.is_file())


def build_changes(root: Path) -> tuple[dict[Path, str], list[Diagnostic], int]:
    source_files = project_source_files(root)
    original = {path: path.read_text(encoding="utf-8") for path in source_files}
    definitions_by_name: dict[str, list[FunctionDefinition]] = defaultdict(list)
    externs: list[FunctionDeclaration] = []

    for path in source_files:
        definitions, declarations = scan_c_file(path)
        for definition in definitions:
            definitions_by_name[definition.name].append(definition)
        externs.extend(declarations)

    diagnostics: list[Diagnostic] = []
    eligible: list[tuple[FunctionDeclaration, FunctionDefinition, Path]] = []
    violation_count = 0

    for declaration in externs:
        owners = definitions_by_name.get(declaration.name, [])
        if not owners:
            continue  # External library or assembly symbol.
        if len(owners) != 1:
            diagnostics.append(
                Diagnostic(
                    declaration.path,
                    declaration.line,
                    f"{declaration.name}: ambiguous definition in "
                    + ", ".join(relative(owner.path, root) for owner in owners),
                )
            )
            violation_count += 1
            continue
        owner = owners[0]
        eligible.append((declaration, owner, corresponding_header(root, owner.path)))
        violation_count += 1

    by_header: dict[Path, list[tuple[FunctionDeclaration, FunctionDefinition]]] = defaultdict(list)
    for declaration, owner, header in eligible:
        by_header[header].append((declaration, owner))

    skipped: set[FunctionDeclaration] = set()
    header_additions: dict[Path, list[str]] = defaultdict(list)
    for header, entries in sorted(by_header.items()):
        header_text = header.read_text(encoding="utf-8") if header.exists() else ""
        already_declared = declared_function_names(header_text)
        entries_by_name: dict[str, list[FunctionDeclaration]] = defaultdict(list)
        for declaration, _owner in entries:
            entries_by_name[declaration.name].append(declaration)

        for name, declarations in sorted(entries_by_name.items()):
            if name in already_declared:
                continue
            variants = {
                normalized_declaration(declaration.text) for declaration in declarations
            }
            if len(variants) != 1:
                for declaration in declarations:
                    skipped.add(declaration)
                    diagnostics.append(
                        Diagnostic(
                            declaration.path,
                            declaration.line,
                            f"{name}: conflicting extern prototypes; header not changed",
                        )
                    )
                continue
            header_additions[header].append(
                declaration_without_extern(declarations[0].text)
            )

    changes: dict[Path, str] = {}
    replacements_by_file: dict[Path, list[tuple[int, int, str]]] = defaultdict(list)
    includes_by_file: dict[Path, set[str]] = defaultdict(set)

    for declaration, owner, header in eligible:
        if declaration in skipped:
            continue
        replacements_by_file[declaration.path].append(
            (declaration.start, declaration.end, "")
        )
        includes_by_file[declaration.path].add(include_name(root, header))

    for path, replacements in replacements_by_file.items():
        changed = apply_replacements(original[path], replacements)
        for name in sorted(includes_by_file[path]):
            changed = add_include(changed, name)
        if changed != original[path]:
            changes[path] = changed

    for header, declarations in sorted(header_additions.items()):
        declarations.sort(key=lambda item: function_name(mask_non_code(item)) or item)
        if header.exists():
            before = header.read_text(encoding="utf-8")
            after = add_header_declarations(before, declarations)
        else:
            before = ""
            after = new_header(declarations)
        if after != before:
            changes[header] = after

    return changes, diagnostics, violation_count


def show_diff(root: Path, path: Path, after: str) -> None:
    before = path.read_text(encoding="utf-8") if path.exists() else ""
    sys.stdout.writelines(
        difflib.unified_diff(
            before.splitlines(keepends=True),
            after.splitlines(keepends=True),
            fromfile=relative(path, root),
            tofile=relative(path, root),
        )
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parent.parent,
        help="project root (default: parent of tools/)",
    )
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--apply", action="store_true", help="write the refactor")
    mode.add_argument(
        "--check",
        action="store_true",
        help="print a summary and fail if project externs remain",
    )
    parser.add_argument(
        "--no-diff",
        action="store_true",
        help="suppress unified diffs in dry-run mode",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root = args.root.resolve()
    if not (root / "src").is_dir() or not (root / "include").is_dir():
        print(f"error: {root} is not a project root with src/ and include/", file=sys.stderr)
        return 2

    changes, diagnostics, violation_count = build_changes(root)
    for diagnostic in diagnostics:
        print(
            f"{relative(diagnostic.path, root)}:{diagnostic.line}: "
            f"warning: {diagnostic.message}",
            file=sys.stderr,
        )

    if args.apply:
        for path, text in sorted(changes.items()):
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text, encoding="utf-8")
        print(
            f"Updated {len(changes)} files for {violation_count} "
            f"project extern declaration(s)."
        )
    elif args.check:
        if violation_count:
            print(
                f"{violation_count} project function extern declaration(s) "
                f"need refactoring across {len(changes)} file(s).",
                file=sys.stderr,
            )
        else:
            print("No project function extern declarations found.")
    else:
        if not args.no_diff:
            for path, text in sorted(changes.items()):
                show_diff(root, path, text)
        print(
            f"Dry run: {violation_count} project extern declaration(s), "
            f"{len(changes)} file(s) would change. Use --apply to write them."
        )

    if args.check and violation_count:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
