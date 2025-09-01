#!/usr/bin/env python3

# SPDX-FileCopyrightText: © 2022 AngheloAlf
# SPDX-License-Identifier: MIT

from __future__ import annotations

import argparse
import mapfile_parser
from pathlib import Path


ASMPATH = Path("asm")


def getProgressFromMapFile(mapFile: mapfile_parser.MapFile, asmPath: Path, aliases: dict[str, str]=dict(), pathIndex: int=2, fullPath: bool=False) -> tuple[mapfile_parser.ProgressStats, dict[str, mapfile_parser.ProgressStats]]:
    totalStats = mapfile_parser.ProgressStats()
    progressPerFolder: dict[str, mapfile_parser.ProgressStats] = dict()

    for segment in mapFile:
        for file in segment:
            if len(file) == 0:
                continue

            folderParts = list(file.filepath.parts[pathIndex:])
            if "src" in folderParts:
                folderParts.remove("src")
            if folderParts[0] in aliases:
                folderParts[0] = aliases[folderParts[0]]
            if not fullPath:
                folderParts = folderParts[:1]
            folder = "/".join(folderParts)

            if folder not in progressPerFolder:
                progressPerFolder[folder] = mapfile_parser.ProgressStats()

            originalFilePath = Path(*file.filepath.parts[pathIndex:])
            fullAsmFile = asmPath / originalFilePath.with_suffix(".s")
            wholeFileIsUndecomped = fullAsmFile.exists()

            for func in file:
                if func.name.endswith(".NON_MATCHING"):
                    continue

                funcNonMatching = f"{func.name}.NON_MATCHING"

                funcSize = 0
                if func.size is not None:
                    funcSize = func.size

                # Check if the function has an assembly file in asm/nonmatchings/
                funcAsmPath = asmPath / "nonmatchings" / originalFilePath.stem / f"{func.name}.s"
                
                if wholeFileIsUndecomped:
                    totalStats.undecompedSize += funcSize
                    progressPerFolder[folder].undecompedSize += funcSize
                elif mapFile.findSymbolByName(funcNonMatching) is not None:
                    totalStats.undecompedSize += funcSize
                    progressPerFolder[folder].undecompedSize += funcSize
                elif funcAsmPath.exists():
                    # Function uses INCLUDE_ASM - count as undecomped
                    totalStats.undecompedSize += funcSize
                    progressPerFolder[folder].undecompedSize += funcSize
                else:
                    totalStats.decompedSize += funcSize
                    progressPerFolder[folder].decompedSize += funcSize

    return totalStats, progressPerFolder

def getProgress(mapPath: Path, version: str, subpaths: bool=False) -> tuple[mapfile_parser.ProgressStats, dict[str, mapfile_parser.ProgressStats]]:
    mapFile = mapfile_parser.MapFile()
    mapFile.readMapFile(mapPath)

    for segment in mapFile:
        for file in segment:
            if len(file) == 0:
                continue

            filepathParts = list(file.filepath.parts)
            if version in filepathParts:
                filepathParts.remove(version)
            if version in filepathParts:
                filepathParts.remove(version)
            file.filepath = Path(*filepathParts)

            # Fix symbol size calculation because of NON_MATCHING symbols
            for sym in file:
                if sym.name.endswith(".NON_MATCHING") and sym.size != 0:
                    realSym = file.findSymbolByName(sym.name.replace(".NON_MATCHING", ""))
                    if realSym is not None and realSym.size == 0:
                        realSym.size = sym.size
                        sym.size = 0

    fullPath = False
    if subpaths:
        fullPath = True

    return getProgressFromMapFile(mapFile.filterBySectionType(".text"), ASMPATH, aliases={"ultralib": "libultra"}, fullPath=fullPath)


def progressMain():
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--version", help="version to process", default="us")
    parser.add_argument("-p", "--subpaths", help="Make a summary for one level deeper in the path tree", action="store_true")
    parser.add_argument("-s", "--sort", help="Sort by decomped size instead of the ROM sorting", action="store_true")
    parser.add_argument("-r", "--remaining", help="Print an extra column indicating the remaining percentage to match of each entry", action="store_true")
    parser.add_argument("-b", "--badge", help="Print raw percentage for use in github badge", action="store_true")

    args = parser.parse_args()

    remaining: bool = args.remaining

    mapPath = "snowboardkids2.map"

    totalStats, progressPerFolder = getProgress(mapPath, args.version, args.subpaths)

    if args.badge:
      update_readme(totalStats.decompedPercentage())
      return

    # Calculate the size for the first column
    columnSize = 27
    for folder in progressPerFolder:
        if len(folder) > columnSize:
            columnSize = len(folder)
    columnSize += 1

    print(mapfile_parser.ProgressStats.getHeaderAsStr(categoryColumnSize=columnSize))
    print(totalStats.getEntryAsStr("all", totalStats, categoryColumnSize=columnSize))
    print()

    progressesList = list(progressPerFolder.items())
    if args.sort:
        progressesList.sort(key=lambda x: (x[1].decompedSize / x[1].total, x[1].total, x[0]), reverse=True)
    for folder, statsEntry in progressesList:
        print(statsEntry.getEntryAsStr(folder, totalStats, categoryColumnSize=columnSize), end="")
        if remaining and statsEntry.undecompedSize != 0:
            remainingPercentage = statsEntry.total / totalStats.total * 100 - statsEntry.decompedPercentageTotal(totalStats)
            print(f"{remainingPercentage:>8.4f}%", end="")
        print()

def update_readme(percentage):
    with open("README.md", 'r') as file:
        lines = file.readlines()
        lines[0] = f"![progress](https://img.shields.io/badge/progress-{percentage:.2f}%25-blue)\n"
 
    with open("README.md", 'w') as file:
        file.writelines(lines)

if __name__ == "__main__":
    progressMain()