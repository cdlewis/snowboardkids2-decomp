#!/usr/bin/env python3

import json
import os
from pathlib import Path

def get_matched_functions():
    """Get all matched functions from asm/matchings directory."""
    matchings_dir = Path("asm/matchings")

    if not matchings_dir.exists():
        return []

    matched_functions = []

    # Walk through all files in asm/matchings and subdirectories
    for root, dirs, files in os.walk(matchings_dir):
        for file in files:
            if file.endswith('.s'):
                # Remove the .s extension to get the function name
                function_name = file[:-2]
                matched_functions.append(function_name)

    # Sort with priority: functions starting with func_ first, then alphabetically
    matched_functions.sort(key=lambda f: (0 if f.startswith('func_') else 1, f))

    return matched_functions

def main():
    functions = get_matched_functions()
    # Output as JSON array for task-runner consumption
    print(json.dumps(functions))

if __name__ == '__main__':
    main()
