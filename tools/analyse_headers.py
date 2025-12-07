#!/usr/bin/env python3
"""
C Header Analysis Script

Analyzes a C project to find:
1. Functions defined in .c files but accessed via extern in other files
2. Missing header inclusions
3. Suggests proper header file locations

Supports a deny list file at tools/difficult-headers to suppress known issues.
Format: file_path:line_number [optional comment]

Usage: python analyze_headers.py <project_directory>
"""

import os
import re
import sys
from collections import defaultdict, namedtuple
from pathlib import Path

# Data structures
FunctionDef = namedtuple('FunctionDef', ['name', 'file', 'line', 'signature', 'is_static'])
ExternDecl = namedtuple('ExternDecl', ['name', 'file', 'line', 'signature'])
HeaderDecl = namedtuple('HeaderDecl', ['name', 'file', 'line', 'signature'])
FunctionDecl = namedtuple('FunctionDecl', ['name', 'file', 'line', 'signature'])

class CProjectAnalyzer:
    def __init__(self, project_dir):
        self.project_dir = Path(project_dir)
        self.function_definitions = {}  # func_name -> FunctionDef
        self.extern_declarations = defaultdict(list)  # func_name -> [ExternDecl]
        self.header_declarations = defaultdict(list)  # func_name -> [HeaderDecl]
        self.file_includes = defaultdict(set)  # file -> set of included headers
        self.c_file_declarations = defaultdict(list)  # func_name -> [FunctionDecl] (declarations in .c files)
        self.deny_list = self._load_deny_list()  # set of (file_path, line_num) tuples to ignore
        
        # Regex patterns
        self.func_def_pattern = re.compile(
            r'^(?!.*static\s+)(\w+(?:\s*\*)*)\s+(\w+)\s*\([^)]*\)\s*\{',
            re.MULTILINE
        )
        self.static_func_pattern = re.compile(
            r'^static\s+\w+(?:\s*\*)*\s+(\w+)\s*\([^)]*\)\s*\{',
            re.MULTILINE
        )
        self.extern_pattern = re.compile(
            r'extern\s+([^;]+;)',
            re.MULTILINE
        )
        self.header_func_pattern = re.compile(
            r'^(?!.*static\s+)(\w+(?:\s*\*)*)\s+(\w+)\s*\([^)]*\)\s*;',
            re.MULTILINE
        )
        self.c_func_decl_pattern = re.compile(
            r'^(?!.*extern\s+)(?!.*static\s+)(\w+(?:\s*\*)*)\s+(\w+)\s*\([^)]*\)\s*;',
            re.MULTILINE
        )
        self.include_pattern = re.compile(
            r'#include\s+[<"]([^>"]+)[>"]'
        )
        self.include_asm_pattern = re.compile(
            r'INCLUDE_ASM\s*\([^,]+,\s*(\w+)\s*\)',
            re.MULTILINE
        )
    
    def _load_deny_list(self):
        """Load deny list from tools/difficult-headers file"""
        deny_list = set()
        deny_list_path = Path('tools/difficult-headers')

        if not deny_list_path.exists():
            return deny_list

        try:
            with open(deny_list_path, 'r') as f:
                for line in f:
                    line = line.strip()
                    if not line or line.startswith('#'):
                        continue

                    # Parse format: "file_path:line_num [optional text]"
                    parts = line.split()
                    if parts:
                        file_line = parts[0]
                        if ':' in file_line:
                            file_path, line_num = file_line.rsplit(':', 1)
                            try:
                                deny_list.add((file_path, int(line_num)))
                            except ValueError:
                                print(f"Warning: Invalid line number in deny list: {line}")
        except Exception as e:
            print(f"Warning: Could not read deny list file: {e}")

        return deny_list

    def _is_denied(self, file_path, line_num):
        """Check if a file:line combination is in the deny list"""
        # Normalize the file path to match against deny list entries
        file_str = str(file_path)

        # Check exact match
        if (file_str, line_num) in self.deny_list:
            return True

        # Also check with relative path from project root
        try:
            rel_path = str(Path(file_str).relative_to(self.project_dir))
            if (rel_path, line_num) in self.deny_list:
                return True
        except ValueError:
            pass

        return False

    def extract_function_name_from_signature(self, signature):
        """Extract function name from a function signature"""
        # Handle cases like "int *func_name(" and "int func_name("
        match = re.search(r'(\w+)\s*\(', signature)
        return match.group(1) if match else None
    
    def is_c_file(self, filepath):
        return filepath.suffix.lower() == '.c'
    
    def is_header_file(self, filepath):
        return filepath.suffix.lower() in ['.h', '.hpp']
    
    def read_file_safe(self, filepath):
        """Safely read file content, handling encoding issues"""
        encodings = ['utf-8', 'latin-1', 'cp1252']
        for encoding in encodings:
            try:
                with open(filepath, 'r', encoding=encoding) as f:
                    return f.read()
            except UnicodeDecodeError:
                continue
        print(f"Warning: Could not read {filepath} with any encoding")
        return ""
    
    def analyze_c_file(self, filepath):
        """Analyze a single C file for function definitions and extern declarations"""
        content = self.read_file_safe(filepath)
        if not content:
            return
        
        # Find includes
        includes = set()
        for match in self.include_pattern.finditer(content):
            includes.add(match.group(1))
        self.file_includes[str(filepath)] = includes
        
        # Find function definitions (non-static)
        lines = content.split('\n')
        for i, line in enumerate(lines, 1):
            # Check for function definition
            if '{' in line and '(' in line:
                # Simple heuristic: if line contains return type, name, params, and opening brace
                stripped = line.strip()
                if (not stripped.startswith('//') and 
                    not stripped.startswith('/*') and
                    not 'static' in stripped and
                    re.match(r'^\w+', stripped)):
                    
                    # Extract function name
                    func_match = re.search(r'(\w+)\s*\(', line)
                    if func_match:
                        func_name = func_match.group(1)
                        # Skip common non-function patterns
                        if func_name not in ['if', 'while', 'for', 'switch', 'sizeof']:
                            # Check if this looks like a function definition
                            paren_pos = line.find('(')
                            if paren_pos > 0:
                                before_paren = line[:paren_pos].strip()
                                # Should have a return type and function name
                                parts = before_paren.split()
                                if len(parts) >= 2:  # return_type function_name
                                    self.function_definitions[func_name] = FunctionDef(
                                        name=func_name,
                                        file=str(filepath),
                                        line=i,
                                        signature=line.strip(),
                                        is_static=False
                                    )
        
        # Find INCLUDE_ASM definitions
        for match in self.include_asm_pattern.finditer(content):
            func_name = match.group(1)
            line_num = content[:match.start()].count('\n') + 1
            # Add as a function definition
            self.function_definitions[func_name] = FunctionDef(
                name=func_name,
                file=str(filepath),
                line=line_num,
                signature=f"INCLUDE_ASM(..., {func_name})",
                is_static=False
            )
        
        # Find extern declarations
        for match in self.extern_pattern.finditer(content):
            extern_decl = match.group(1).strip()
            func_name = self.extract_function_name_from_signature(extern_decl)
            if func_name:
                line_num = content[:match.start()].count('\n') + 1
                self.extern_declarations[func_name].append(ExternDecl(
                    name=func_name,
                    file=str(filepath),
                    line=line_num,
                    signature=extern_decl
                ))
        
        # Find function declarations (non-extern) in C files
        for match in self.c_func_decl_pattern.finditer(content):
            func_signature = match.group(0)
            func_name = self.extract_function_name_from_signature(func_signature)
            if func_name and func_name not in ['if', 'while', 'for', 'switch', 'sizeof']:
                line_num = content[:match.start()].count('\n') + 1
                self.c_file_declarations[func_name].append(FunctionDecl(
                    name=func_name,
                    file=str(filepath),
                    line=line_num,
                    signature=func_signature
                ))
        
        
    
    def analyze_header_file(self, filepath):
        """Analyze a header file for function declarations"""
        content = self.read_file_safe(filepath)
        if not content:
            return
        
        # Find function declarations
        for match in self.header_func_pattern.finditer(content):
            func_signature = match.group(0)
            func_name = self.extract_function_name_from_signature(func_signature)
            if func_name:
                line_num = content[:match.start()].count('\n') + 1
                self.header_declarations[func_name].append(HeaderDecl(
                    name=func_name,
                    file=str(filepath),
                    line=line_num,
                    signature=func_signature
                ))
    
    def scan_project(self):
        """Scan the entire project directory"""
        print(f"Scanning project directory: {self.project_dir}")
        
        c_files = []
        h_files = []
        
        for filepath in self.project_dir.rglob('*'):
            if filepath.is_file():
                if self.is_c_file(filepath):
                    # Skip temp.c file
                    if filepath.name == 'temp.c' and filepath.parent.name == 'src':
                        continue
                    c_files.append(filepath)
                elif self.is_header_file(filepath):
                    h_files.append(filepath)
        
        print(f"Found {len(c_files)} C files and {len(h_files)} header files")
        
        # Analyze all files
        for filepath in c_files:
            self.analyze_c_file(filepath)
        
        for filepath in h_files:
            self.analyze_header_file(filepath)
    
    def suggest_header_file(self, func_name, definition_file):
        """Suggest appropriate header file for a function"""
        def_path = Path(definition_file)
        
        # First try: look for corresponding .h file
        potential_header = def_path.with_suffix('.h')
        if potential_header.exists():
            return str(potential_header)
        
        # Second try: look for headers in the same directory
        header_files = list(def_path.parent.glob('*.h'))
        if header_files:
            # Prefer headers with similar names
            def_stem = def_path.stem
            for header in header_files:
                if def_stem in header.stem or header.stem in def_stem:
                    return str(header)
            # Otherwise return the first one
            return str(header_files[0])
        
        # Third try: suggest creating a new header
        return str(def_path.with_suffix('.h'))
    
    def generate_report(self, limit=None):
        """Generate analysis report"""
        print("\n" + "="*80)
        print("C PROJECT HEADER ANALYSIS REPORT")
        print("="*80)

        # Find functions that need proper headers
        issues = []
        denied_count = 0
        
        # Check for function declarations in C files that have definitions elsewhere
        # These should be moved to header files
        for func_name, c_decls in self.c_file_declarations.items():
            # Only flag if this function HAS a definition somewhere (including INCLUDE_ASM)
            if func_name in self.function_definitions:
                definition = self.function_definitions[func_name]
                # Check each declaration
                for c_decl in c_decls:
                    # If declaration is in a different file than the definition, it should be in a header
                    if c_decl.file != definition.file:
                        # Skip if this is in the deny list
                        if self._is_denied(c_decl.file, c_decl.line):
                            denied_count += 1
                        else:
                            issues.append({
                                'type': 'declaration_should_be_in_header',
                                'function': func_name,
                                'declaration': c_decl,
                                'definition': definition
                            })
        
        for func_name, extern_decls in self.extern_declarations.items():
            # Check if this function is defined somewhere
            if func_name in self.function_definitions:
                definition = self.function_definitions[func_name]
                
                # Check if it's already properly declared in a header
                has_header_decl = func_name in self.header_declarations
                
                if not has_header_decl:
                    # Check if all extern uses are in the same file as the definition
                    extern_files = set(decl.file for decl in extern_decls)
                    if not (len(extern_files) == 1 and definition.file in extern_files):
                        # Filter out extern declarations that are in the deny list
                        filtered_extern_decls = []
                        for decl in extern_decls:
                            if self._is_denied(decl.file, decl.line):
                                denied_count += 1
                            else:
                                filtered_extern_decls.append(decl)

                        # Only report if there are still extern uses after filtering
                        if filtered_extern_decls:
                            # Function is defined but not in any header, and used via extern in other files
                            suggested_header = self.suggest_header_file(func_name, definition.file)
                            issues.append({
                                'type': 'missing_header_declaration',
                                'function': func_name,
                                'definition': definition,
                                'extern_uses': filtered_extern_decls,
                                'suggested_header': suggested_header
                            })
                else:
                    # Function has header declaration, check if extern users include it
                    header_files = [decl.file for decl in self.header_declarations[func_name]]
                    for extern_decl in extern_decls:
                        # Skip if this extern declaration is in the deny list
                        if self._is_denied(extern_decl.file, extern_decl.line):
                            denied_count += 1
                            continue

                        extern_file_includes = self.file_includes.get(extern_decl.file, set())

                        # Check if any of the headers containing this function are included
                        header_included = any(
                            os.path.basename(hf) in extern_file_includes or
                            hf in extern_file_includes
                            for hf in header_files
                        )

                        if not header_included:
                            issues.append({
                                'type': 'missing_include',
                                'function': func_name,
                                'extern_use': extern_decl,
                                'available_headers': header_files
                            })
        
        # Print summary
        print(f"\nSUMMARY:")
        print(f"Functions defined: {len(self.function_definitions)}")
        print(f"Extern declarations found: {sum(len(decls) for decls in self.extern_declarations.values())}")
        print(f"Header declarations found: {sum(len(decls) for decls in self.header_declarations.values())}")
        print(f"Issues found: {len(issues)}")
        if denied_count > 0:
            print(f"Issues filtered by deny list: {denied_count}")
        if limit is not None and len(issues) > limit:
            print(f"Showing only first {limit} issues (use --limit to adjust)")
        
        # Print detailed issues
        if issues:
            print(f"\nISSUES FOUND:")
            print("-" * 80)

            # Apply limit if specified
            issues_to_show = issues[:limit] if limit is not None else issues

            for i, issue in enumerate(issues_to_show, 1):
                print(f"\n{i}. {issue['type'].upper().replace('_', ' ')}")
                
                if issue['type'] == 'missing_header_declaration':
                    func = issue['function']
                    definition = issue['definition']
                    print(f"   Function: {func}")
                    print(f"   Defined in: {definition.file}:{definition.line}")
                    print(f"   Used via extern in:")
                    for extern_decl in issue['extern_uses']:
                        print(f"     - {extern_decl.file}:{extern_decl.line}")
                    print(f"   RECOMMENDATION: Add declaration to {issue['suggested_header']}")
                    if "INCLUDE_ASM" in definition.signature:
                        # Don't suggest adding INCLUDE_ASM to header
                        print(f"   Add appropriate function declaration for {func}")
                    else:
                        print(f"   Add this line: {definition.signature.replace('{', ';')}")
                
                elif issue['type'] == 'missing_include':
                    func = issue['function']
                    extern_use = issue['extern_use']
                    print(f"   Function: {func}")
                    print(f"   File with extern: {extern_use.file}:{extern_use.line}")
                    print(f"   Available in headers: {', '.join(issue['available_headers'])}")
                    print(f"   RECOMMENDATION: Add #include for appropriate header")
                
                elif issue['type'] == 'declaration_should_be_in_header':
                    func = issue['function']
                    declaration = issue['declaration']
                    definition = issue['definition']
                    print(f"   Function: {func}")
                    print(f"   Declaration in: {declaration.file}:{declaration.line}")
                    print(f"   Definition in: {definition.file}:{definition.line}")
                    print(f"   RECOMMENDATION: Move declaration to appropriate header file")
        
        else:
            print(f"\n✅ No issues found! All functions are properly declared in headers.")
        
        print(f"\n" + "="*80)

def main():
    import argparse

    parser = argparse.ArgumentParser(description='Analyze C project headers')
    parser.add_argument('project_directory', help='Path to the project directory')
    parser.add_argument('--limit', type=int, default=None,
                        help='Limit the number of issues shown in the report')

    args = parser.parse_args()

    if not os.path.isdir(args.project_directory):
        print(f"Error: {args.project_directory} is not a valid directory")
        sys.exit(1)

    analyzer = CProjectAnalyzer(args.project_directory)
    analyzer.scan_project()
    analyzer.generate_report(limit=args.limit)

if __name__ == "__main__":
    main()
