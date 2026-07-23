#!/usr/bin/env python3
"""Group same-file C function forward declarations into one block.

The first movable forward declaration in each file is used as the anchor. Later
declarations are moved to that location in source order. This preserves the
usual project layout (types/globals, declarations, implementations) without
guessing where a new declaration section should be created.

Extern-only declarations and declarations inside preprocessor conditionals are
left alone. A declaration is also left in place if moving it would put it
before a typedef used by its signature.
"""

from __future__ import annotations

import argparse
import difflib
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


IDENTIFIER_RE = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
DECLARATOR_RE = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\(")
CONDITIONAL_DIRECTIVES = {"if", "ifdef", "ifndef"}
NON_FUNCTION_NAMES = {
    "__attribute__",
    "__declspec",
    "__asm__",
    "alignas",
    "if",
    "for",
    "switch",
    "while",
    "sizeof",
}


@dataclass(frozen=True)
class Statement:
    start: int
    end: int
    conditional_depth: int
    masked_text: str


@dataclass(frozen=True)
class Declaration:
    statement: Statement
    text: str


def matching_paren(text: str, opening: int) -> int:
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "(":
            depth += 1
        elif text[index] == ")":
            depth -= 1
            if depth == 0:
                return index
    return len(text) - 1


def declarator_name(code: str) -> str | None:
    position = 0
    while match := DECLARATOR_RE.search(code, position):
        name = match.group(1)
        if name in NON_FUNCTION_NAMES:
            position = matching_paren(code, match.end() - 1) + 1
            continue
        if name.isupper():
            return None
        prefix = code[: match.start(1)].strip()
        return name if prefix and IDENTIFIER_RE.search(prefix) else None
    return None


def looks_like_function_header(code: str) -> bool:
    stripped = code.strip()
    return (
        "=" not in stripped
        and not stripped.startswith("typedef ")
        and re.search(r"\(\s*\*", stripped) is None
        and declarator_name(stripped) is not None
    )


def mask_non_code(text: str) -> tuple[str, list[int]]:
    """Mask comments, literals, and directives while retaining source offsets."""
    masked = list(text)
    conditional_depths = [0] * (len(text) + 1)
    conditional_depth = 0
    in_block_comment = False
    in_directive_continuation = False

    offset = 0
    for line in text.splitlines(keepends=True):
        stripped = line.lstrip()
        is_directive = in_directive_continuation or (not in_block_comment and stripped.startswith("#"))

        if is_directive:
            depth_for_line = conditional_depth
            if not in_directive_continuation:
                directive_match = re.match(r"#\s*([A-Za-z_][A-Za-z0-9_]*)", stripped)
                directive = directive_match.group(1) if directive_match else ""
                if directive == "endif":
                    conditional_depth = max(0, conditional_depth - 1)
                    depth_for_line = conditional_depth
                elif directive in CONDITIONAL_DIRECTIVES:
                    depth_for_line = conditional_depth
                    conditional_depth += 1

            for index in range(offset, offset + len(line)):
                conditional_depths[index] = depth_for_line
                if masked[index] != "\n":
                    masked[index] = " "
            in_directive_continuation = line.rstrip("\r\n").endswith("\\")
            offset += len(line)
            continue

        in_directive_continuation = False
        state = "block_comment" if in_block_comment else "code"
        index = offset
        line_end = offset + len(line)

        while index < line_end:
            conditional_depths[index] = conditional_depth
            char = text[index]
            next_char = text[index + 1] if index + 1 < line_end else ""

            if state == "code":
                if char == "/" and next_char == "/":
                    state = "line_comment"
                    masked[index] = masked[index + 1] = " "
                    index += 2
                    continue
                if char == "/" and next_char == "*":
                    state = "block_comment"
                    masked[index] = masked[index + 1] = " "
                    index += 2
                    continue
                if char == '"':
                    state = "string"
                    masked[index] = " "
                elif char == "'":
                    state = "char"
                    masked[index] = " "
            elif state == "line_comment":
                if char != "\n":
                    masked[index] = " "
            elif state == "block_comment":
                if char == "*" and next_char == "/":
                    masked[index] = masked[index + 1] = " "
                    index += 2
                    state = "code"
                    continue
                if char != "\n":
                    masked[index] = " "
            elif state in {"string", "char"}:
                delimiter = '"' if state == "string" else "'"
                if char == "\\":
                    masked[index] = " "
                    if index + 1 < line_end:
                        if masked[index + 1] != "\n":
                            masked[index + 1] = " "
                        index += 2
                        continue
                if char == delimiter:
                    state = "code"
                if char != "\n":
                    masked[index] = " "

            index += 1

        in_block_comment = state == "block_comment"
        offset = line_end

    conditional_depths[len(text)] = conditional_depth
    return "".join(masked), conditional_depths


def top_level_statements(text: str) -> list[Statement]:
    masked, conditional_depths = mask_non_code(text)
    statements = []
    brace_depth = 0
    boundary = 0
    outer_brace_is_function = False

    for index, char in enumerate(masked):
        if char == "{":
            if brace_depth == 0:
                outer_brace_is_function = looks_like_function_header(masked[boundary:index])
            brace_depth += 1
        elif char == "}":
            brace_depth = max(0, brace_depth - 1)
            if brace_depth == 0 and outer_brace_is_function:
                boundary = index + 1
                outer_brace_is_function = False
        elif char == ";" and brace_depth == 0:
            start = boundary
            while start < index and masked[start].isspace():
                start += 1
            if start < index:
                statements.append(
                    Statement(
                        start=start,
                        end=index + 1,
                        conditional_depth=conditional_depths[start],
                        masked_text=masked[start : index + 1],
                    )
                )
            boundary = index + 1

    return statements


def function_definitions(text: str) -> set[str]:
    masked, _ = mask_non_code(text)
    definitions = set()
    brace_depth = 0
    boundary = 0
    outer_brace_is_function = False

    for index, char in enumerate(masked):
        if char == "{":
            if brace_depth == 0:
                header = masked[boundary:index]
                outer_brace_is_function = looks_like_function_header(header)
                if outer_brace_is_function:
                    name = declarator_name(header)
                    if name is not None:
                        definitions.add(name)
            brace_depth += 1
        elif char == "}":
            brace_depth = max(0, brace_depth - 1)
            if brace_depth == 0 and outer_brace_is_function:
                boundary = index + 1
                outer_brace_is_function = False
        elif char == ";" and brace_depth == 0:
            boundary = index + 1

    return definitions


def function_name(statement: Statement) -> str | None:
    code = statement.masked_text.strip()
    if (
        statement.conditional_depth != 0
        or not code.endswith(";")
        or "typedef" in IDENTIFIER_RE.findall(code)
        or "=" in code
        or re.search(r"\(\s*\*", code)
    ):
        return None

    return declarator_name(code)


def typedef_names(statements: list[Statement]) -> dict[str, int]:
    names = {}
    for statement in statements:
        code = statement.masked_text.strip()
        if not code.startswith("typedef "):
            continue

        pointer_match = re.search(r"\(\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)", code)
        identifiers = IDENTIFIER_RE.findall(code[:-1])
        if pointer_match:
            names[pointer_match.group(1)] = statement.start
        elif identifiers:
            names[identifiers[-1]] = statement.start
    return names


def full_line_span(text: str, start: int, end: int) -> tuple[int, int]:
    line_start = text.rfind("\n", 0, start) + 1
    line_end = text.find("\n", end)
    if line_end == -1:
        line_end = len(text)
    else:
        line_end += 1

    before = text[line_start:start]
    after = text[end:line_end].strip()
    if before.strip() or (after and not after.startswith("//")):
        return start, end
    return line_start, line_end


def consume_blank_lines(text: str, offset: int) -> int:
    while offset < len(text):
        line_end = text.find("\n", offset)
        if line_end == -1:
            line_end = len(text)
        else:
            line_end += 1
        if text[offset:line_end].strip():
            break
        offset = line_end
    return offset


def group_declarations(text: str) -> tuple[str, list[str]]:
    statements = top_level_statements(text)
    typedefs = typedef_names(statements)
    definitions = function_definitions(text)
    candidates = [
        Declaration(statement=statement, text=text[statement.start : statement.end].strip())
        for statement in statements
        if function_name(statement) in definitions
    ]
    if len(candidates) < 2:
        return text, []

    anchor = candidates[0]
    movable = [anchor]
    skipped = []

    for declaration in candidates[1:]:
        identifiers = set(IDENTIFIER_RE.findall(declaration.statement.masked_text))
        late_types = sorted(
            name
            for name in identifiers
            if anchor.statement.start < typedefs.get(name, -1) < declaration.statement.start
        )
        if late_types:
            skipped.append(
                f"line {text.count(chr(10), 0, declaration.statement.start) + 1}: "
                f"uses later typedef(s) {', '.join(late_types)}"
            )
        else:
            movable.append(declaration)

    if len(movable) < 2:
        return text, skipped

    replacements: list[tuple[int, int, str]] = []
    anchor_start, anchor_end = full_line_span(text, anchor.statement.start, anchor.statement.end)
    anchor_end = consume_blank_lines(text, anchor_end)
    declaration_block = "\n".join(declaration.text for declaration in movable) + "\n\n"
    replacements.append((anchor_start, anchor_end, declaration_block))

    for declaration in movable[1:]:
        start, end = full_line_span(text, declaration.statement.start, declaration.statement.end)
        end = consume_blank_lines(text, end)
        replacements.append((start, end, ""))

    result = text
    for start, end, replacement in sorted(replacements, reverse=True):
        result = result[:start] + replacement + result[end:]
    return result, skipped


def source_files(paths: list[Path]) -> list[Path]:
    files = set()
    for path in paths:
        if path.is_dir():
            for candidate in path.rglob("*.c"):
                if not candidate.is_file():
                    continue
                ignored = subprocess.run(
                    ["git", "check-ignore", "--quiet", "--", str(candidate)],
                    check=False,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                )
                if ignored.returncode != 0:
                    files.add(candidate)
        elif path.suffix == ".c":
            files.add(path)
        else:
            print(f"Skipping non-C file: {path}", file=sys.stderr)
    return sorted(files)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="+", type=Path, help="C files or directories to process")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--in-place", action="store_true", help="rewrite changed files")
    mode.add_argument("--check", action="store_true", help="return nonzero if files need cleanup")
    args = parser.parse_args()

    changed = 0
    for path in source_files(args.paths):
        original = path.read_text()
        grouped, skipped = group_declarations(original)
        for reason in skipped:
            print(f"{path}: skipped declaration at {reason}", file=sys.stderr)
        if grouped == original:
            continue

        changed += 1
        if args.in_place:
            path.write_text(grouped)
        elif not args.check:
            diff = difflib.unified_diff(
                original.splitlines(keepends=True),
                grouped.splitlines(keepends=True),
                fromfile=str(path),
                tofile=str(path),
            )
            sys.stdout.writelines(diff)

    if args.in_place:
        print(f"Updated {changed} file(s).", file=sys.stderr)
    elif args.check and changed:
        print(f"{changed} file(s) need declaration cleanup.", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
