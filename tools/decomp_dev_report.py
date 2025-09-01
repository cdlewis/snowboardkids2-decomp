#!/usr/bin/env python3

import json
import argparse
import mapfile_parser
from pathlib import Path

def generate_accurate_report(map_path="snowboardkids2.map", output_path="report.json"):
    """Generate an accurate objdiff report using the same logic as progress.py"""
    
    # Parse the map file (same as progress.py)
    mapFile = mapfile_parser.MapFile()
    mapFile.readMapFile(map_path)
    
    # Fix symbol size calculation (same as progress.py)
    for segment in mapFile:
        for file in segment:
            if len(file) == 0:
                continue
            
            for sym in file:
                if sym.name.endswith(".NON_MATCHING") and sym.size != 0:
                    realSym = file.findSymbolByName(sym.name.replace(".NON_MATCHING", ""))
                    if realSym is not None and realSym.size == 0:
                        realSym.size = sym.size
                        sym.size = 0
    
    # Initialize counters
    total_code_size = 0
    matched_code_size = 0
    total_functions = 0
    matched_functions = 0
    units = []
    
    asm_path = Path("asm")
    pathIndex = 2
    
    # Process each segment (same logic as progress.py getProgressFromMapFile)
    for segment in mapFile.filterBySectionType(".text"):
        for file in segment:
            if len(file) == 0:
                continue
            
            originalFilePath = Path(*file.filepath.parts[pathIndex:])
            fullAsmFile = asm_path / originalFilePath.with_suffix(".s")
            wholeFileIsUndecomped = fullAsmFile.exists()
            
            unit_total_code = 0
            unit_matched_code = 0
            unit_total_functions = 0
            unit_matched_functions = 0
            
            # Process each function (same logic as progress.py)
            for func in file:
                if func.name.endswith(".NON_MATCHING"):
                    continue
                
                funcNonMatching = f"{func.name}.NON_MATCHING"
                
                funcSize = 0
                if func.size is not None:
                    funcSize = func.size
                
                # Check if the function has an assembly file in asm/nonmatchings/
                funcAsmPath = asm_path / "nonmatchings" / originalFilePath.stem / f"{func.name}.s"
                
                # Same logic as progress.py
                is_matched = True
                if wholeFileIsUndecomped:
                    is_matched = False
                elif mapFile.findSymbolByName(funcNonMatching) is not None:
                    is_matched = False
                elif funcAsmPath.exists():
                    # Function uses INCLUDE_ASM - count as undecomped
                    is_matched = False
                
                # Update counters
                unit_total_code += funcSize
                total_code_size += funcSize
                unit_total_functions += 1
                total_functions += 1
                
                if is_matched:
                    unit_matched_code += funcSize
                    matched_code_size += funcSize
                    unit_matched_functions += 1
                    matched_functions += 1
            
            # Create unit entry for this file (only if it has functions)
            if unit_total_functions > 0:
                unit = {
                    "name": str(file.filepath),
                    "measures": {
                        "fuzzy_match_percent": (unit_matched_code / unit_total_code * 100) if unit_total_code > 0 else 0,
                        "total_code": str(unit_total_code),
                        "matched_code": str(unit_matched_code),
                        "matched_code_percent": (unit_matched_code / unit_total_code * 100) if unit_total_code > 0 else 0,
                        "matched_data_percent": 100.0,  # Assuming data is matched
                        "total_functions": unit_total_functions,
                        "matched_functions": unit_matched_functions,
                        "matched_functions_percent": (unit_matched_functions / unit_total_functions * 100) if unit_total_functions > 0 else 0,
                        "total_units": 1
                    },
                    "sections": [
                        {
                            "name": ".text",
                            "size": str(unit_total_code),
                            "metadata": {
                                "virtual_address": "0"
                            }
                        }
                    ],
                    "metadata": {
                        "module_name": originalFilePath.stem,
                        "module_id": len(units),
                        "progress_categories": ["build"]
                    }
                }
                units.append(unit)
    
    # Generate the report
    report = {
        "measures": {
            "fuzzy_match_percent": (matched_code_size / total_code_size * 100) if total_code_size > 0 else 0,
            "total_code": str(total_code_size),
            "matched_code": str(matched_code_size),
            "matched_code_percent": (matched_code_size / total_code_size * 100) if total_code_size > 0 else 0,
            "matched_data_percent": 100.0,  # Assuming data is matched
            "total_functions": total_functions,
            "matched_functions": matched_functions,
            "matched_functions_percent": (matched_functions / total_functions * 100) if total_functions > 0 else 0,
            "complete_data_percent": 100.0,
            "total_units": len(units)
        },
        "units": units
    }
    
    # Write the report
    with open(output_path, 'w') as f:
        json.dump(report, f, indent=2)
    
    # Print summary
    print(f"Generated accurate report: {output_path}")
    print(f"Total code: {total_code_size} bytes")
    print(f"Matched code: {matched_code_size} bytes ({matched_code_size / total_code_size * 100:.4f}%)")
    print(f"Total functions: {total_functions}")
    print(f"Matched functions: {matched_functions} ({matched_functions / total_functions * 100:.4f}%)")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate an accurate objdiff report")
    parser.add_argument("-m", "--map", default="snowboardkids2.map", help="Path to the map file")
    parser.add_argument("-o", "--output", default="report.json", help="Output report file")
    
    args = parser.parse_args()
    generate_accurate_report(args.map, args.output)