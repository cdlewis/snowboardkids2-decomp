# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a matching decompilation project for Snowboard Kids 2 (N64). The goal is to create C code that, when compiled, produces the exact same assembly as the original game ROM.

## Project Structure

- `src` decompiled (or partially decompiled) C code
- `include` headers for decompiled C code
- `asm/nonmatchings` unmatched asm code extracted from the rom. Each file contains a separate function.
- `asm/matchings` decompiled assembly code for already matched C functions. We keep this around as it's sometimes convenient to inspect.
- `lib` library code such as Ultralib which we call and link against
- `assets` binary asset blobs extracted from the rom
- `include` common headers included in all C and/or assembly code
