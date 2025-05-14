# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a matching decompilation project for Snowboard Kids 2 (N64). The goal is to create C code that, when compiled, produces the exact same assembly as the original game ROM.

## Project Structure

* `src` decompiled (or partially decompiled) C code
* `asm` unmatched asm code extracted from the rom
* `lib` library code such as Ultralib which we call and link against
* `assets` binary asset blobs extracted from the rom
* `include` common headers included in all C and/or assembly code

## Tasks

### Decompile assembly to C code

You may be given raw assembly and asked to decompile it to C code.

Analyse the assembly code carefully. Keep in mind The compiler is GCC 2.7.2. The following compiler flags are being used: -O2 -mips3. We follow the C89 standard. Keep in mind that variable declarations are generally limited to the beginning of blocks or functions.

Use the entire project's to find the best possible match, this includes:

* How the function is called in existing C code (src or lib folders)
* How the function calls existing C code
* How the function is called in assembly code (asm folder)
* How the function calls existing assembly code
