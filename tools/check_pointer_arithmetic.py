#!/usr/bin/env python3
"""
Detect pointer arithmetic with casts in C code.

This linter detects patterns like:
    (u8*)ptr + offset
    (SomeStruct*)ptr + index
    *(s32*)((char*)ptr + 0x10)

These should be replaced with proper struct field access.
"""

import sys
import os
import argparse
from pycparser import c_parser, c_ast, parse_file
from pycparser.plyparser import ParseError


class PointerArithmeticDetector(c_ast.NodeVisitor):
    """AST visitor that detects pointer arithmetic with casts."""

    def __init__(self, filename):
        self.filename = filename
        self.violations = []

    def visit_BinaryOp(self, node):
        """Check binary operations for cast + pointer arithmetic pattern."""
        # Check if this is addition or subtraction
        if node.op in ['+', '-']:
            # Check if left operand is a cast
            if isinstance(node.left, c_ast.Cast):
                # Check if the cast is to a pointer type
                if self._is_pointer_cast(node.left):
                    # This is a violation: (Type*)expr + offset
                    self.violations.append({
                        'line': node.coord.line,
                        'column': node.coord.column,
                        'type': self._get_cast_type(node.left),
                        'coord': node.coord
                    })

        # Continue traversing the tree
        self.generic_visit(node)

    def _is_pointer_cast(self, cast_node):
        """Check if a cast node is casting to a pointer type."""
        type_node = cast_node.to_type

        # Navigate through type declarations to find PtrDecl
        while type_node:
            if isinstance(type_node, c_ast.PtrDecl):
                return True
            if isinstance(type_node, c_ast.TypeDecl):
                return False
            if isinstance(type_node, c_ast.Typename):
                type_node = type_node.type
            else:
                break

        return False

    def _get_cast_type(self, cast_node):
        """Extract the type name from a cast for reporting."""
        type_node = cast_node.to_type

        # Try to build a readable type string
        if isinstance(type_node, c_ast.Typename):
            return self._typename_to_string(type_node)

        return "unknown type"

    def _typename_to_string(self, typename):
        """Convert a typename AST node to a string."""
        parts = []

        # Get the base type names
        if typename.type:
            parts.extend(self._get_type_names(typename.type))

        # Add pointer indicator
        if self._has_pointer(typename.type):
            parts.append('*')

        return ''.join(parts) if parts else 'unknown'

    def _get_type_names(self, node):
        """Recursively extract type name strings."""
        names = []

        if isinstance(node, c_ast.IdentifierType):
            names.extend(node.names)
        elif isinstance(node, c_ast.TypeDecl):
            if node.type:
                names.extend(self._get_type_names(node.type))
        elif isinstance(node, c_ast.PtrDecl):
            if node.type:
                names.extend(self._get_type_names(node.type))
        elif isinstance(node, c_ast.Struct):
            if node.name:
                names.append(f'struct {node.name}')
            else:
                names.append('struct')

        return names

    def _has_pointer(self, node):
        """Check if type includes a pointer."""
        if isinstance(node, c_ast.PtrDecl):
            return True
        if isinstance(node, c_ast.TypeDecl):
            return False
        if hasattr(node, 'type'):
            return self._has_pointer(node.type)
        return False


def preprocess_file(filename, cpp_path='gcc', cpp_args=''):
    """
    Preprocess a C file for parsing.

    Uses gcc preprocessor to handle includes and macros.
    """
    # Match the Makefile's include paths (IINC)
    include_paths = [
        'include',
        'lib/ultralib/include',
        'lib/ultralib/include/PR',
        'lib/libmus/include/PR',
        'lib/libmus/src',
        'lib/f3dex2/PR',
        'src',
    ]

    # Match the Makefile's macros (MACROS)
    cpp_args_list = [
        '-E',  # Preprocess only
        '-D_LANGUAGE_C',
        '-D_MIPS_SZLONG=32',
        '-D_MIPS_SZINT=32',
        '-D__USE_ISOC99',
        '-DF3DEX_GBI_2',
        '-DNDEBUG',
        '-D_FINALROM',
        # pycparser compatibility macros
        '-D__attribute__(x)=',
        '-D__extension__=',
        '-D__asm__(x)=',
        '-D__restrict=',
        '-D__volatile__=volatile',
        '-D__inline=',
        '-D__const=const',
    ]

    # Add include paths
    for path in include_paths:
        if os.path.exists(path):
            cpp_args_list.append(f'-I{path}')

    # pycparser expects cpp_args as a list when passed to parse_file
    try:
        return parse_file(filename, use_cpp=True, cpp_path=cpp_path, cpp_args=cpp_args_list)
    except ParseError as e:
        print(f"Warning: Could not parse {filename}: {e}", file=sys.stderr)
        return None


def check_file(filename, verbose=False):
    """Check a single C file for pointer arithmetic violations."""
    if verbose:
        print(f"Checking {filename}...", file=sys.stderr)

    try:
        ast = preprocess_file(filename)
        if ast is None:
            return []

        detector = PointerArithmeticDetector(filename)
        detector.visit(ast)

        return detector.violations

    except Exception as e:
        if verbose:
            print(f"Error processing {filename}: {e}", file=sys.stderr)
        return []


def check_directory(directory, exclude_dirs=None, verbose=False):
    """Recursively check all C files in a directory."""
    if exclude_dirs is None:
        exclude_dirs = {'build', 'tools', 'lib', 'expected', '.git'}

    all_violations = {}

    for root, dirs, files in os.walk(directory):
        # Remove excluded directories from traversal
        dirs[:] = [d for d in dirs if d not in exclude_dirs]

        for filename in files:
            if filename.endswith('.c'):
                filepath = os.path.join(root, filename)
                violations = check_file(filepath, verbose=verbose)

                if violations:
                    all_violations[filepath] = violations

    return all_violations


def format_violation(filename, violation):
    """Format a violation for display."""
    return f"{filename}:{violation['line']}:{violation.get('column', 0)}: " \
           f"pointer arithmetic with cast: ({violation['type']})ptr + offset"


def main():
    parser = argparse.ArgumentParser(
        description='Check for pointer arithmetic with casts in C code',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Check a single file
  %(prog)s src/main.c

  # Check all C files in src directory
  %(prog)s src/

  # Check with verbose output
  %(prog)s src/ --verbose

  # Fail on any violations
  %(prog)s src/ --strict
"""
    )

    parser.add_argument('path', help='File or directory to check')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Show detailed progress')
    parser.add_argument('--strict', action='store_true',
                       help='Exit with error code if violations found')
    parser.add_argument('--exclude', action='append', default=[],
                       help='Additional directories to exclude (can be used multiple times)')

    args = parser.parse_args()

    # Determine if we're checking a file or directory
    if os.path.isfile(args.path):
        violations = check_file(args.path, verbose=args.verbose)
        all_violations = {args.path: violations} if violations else {}
    elif os.path.isdir(args.path):
        exclude_dirs = {'build', 'tools', 'lib', 'expected', '.git'} | set(args.exclude)
        all_violations = check_directory(args.path, exclude_dirs=exclude_dirs, verbose=args.verbose)
    else:
        print(f"Error: {args.path} is not a file or directory", file=sys.stderr)
        return 1

    # Report results
    total_violations = sum(len(v) for v in all_violations.values())

    if total_violations == 0:
        print(f"\n{'='*70}")
        print("Pointer Arithmetic Check")
        print(f"{'='*70}")
        print("✅ No violations found!")
        return 0

    # Print violations
    print(f"\n{'='*70}")
    print("Pointer Arithmetic Check")
    print(f"{'='*70}")
    print(f"Found {total_violations} violation(s) in {len(all_violations)} file(s):\n")

    for filepath in sorted(all_violations.keys()):
        for violation in all_violations[filepath]:
            print(format_violation(filepath, violation))

    print(f"\n{'='*70}")
    print("These patterns should be replaced with proper struct field access.")
    print("See CLAUDE.md for examples of correct struct usage.")
    print(f"{'='*70}\n")

    if args.strict:
        return 1

    return 0


if __name__ == '__main__':
    sys.exit(main())
