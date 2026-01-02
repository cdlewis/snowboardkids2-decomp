#!/usr/bin/env python3

import json
import os
from collections import defaultdict
from pathlib import Path

def get_matched_functions_by_file():
    """Get unnamed matched functions from asm/matchings directory, grouped by source file."""
    matchings_dir = Path("asm/matchings")

    if not matchings_dir.exists():
        return {}

    functions_by_file = defaultdict(list)

    # Walk through all files in asm/matchings and subdirectories
    for root, dirs, files in os.walk(matchings_dir):
        for file in files:
            if file.endswith('.s'):
                # Remove the .s extension to get the function name
                function_name = file[:-2]
                # Only include unnamed functions (starting with func_)
                if not function_name.startswith('func_'):
                    continue
                # Get the parent directory name (source file)
                source_file = Path(root).name
                functions_by_file[source_file].append(function_name)

    # Sort functions within each file alphabetically
    for source_file in functions_by_file:
        functions_by_file[source_file].sort()

    return dict(functions_by_file)

def main():
    functions_by_file = get_matched_functions_by_file()

    # Sort files by number of functions (descending) to show densest files first
    sorted_files = sorted(
        functions_by_file.items(),
        key=lambda x: (-len(x[1]), x[0])
    )

    # Flatten into array, preserving file grouping order
    functions = []
    for _, funcs in sorted_files:
        functions.extend(funcs)

    print(json.dumps(functions))

if __name__ == '__main__':
    main()
