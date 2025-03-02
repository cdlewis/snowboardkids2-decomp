#!/usr/bin/env python3

import argparse
from pathlib import Path

def apply(config: dict, args: argparse.Namespace):
    config["baseimg"] = f"snowboardkids2.z64"
    config["myimg"]   = f"build/snowboardkids2.z64"
    config["mapfile"] = f"snowboardkids2.map"
    config["source_directories"] = ["./src", "./include", "./asm", "./lib"]
    config["objdump_flags"] = ["-Mreg-names=32"]
    config["makeflags"] = ["KEEP_MDEBUG=1", "WERROR=0", "OBJDUMP_BUILD=0", "PERMUTER=1"]
