![progress](https://img.shields.io/badge/progress-19.48%25-blue)

# Snowboard Kids 2

![box art](Snowboard_Kids_2_cover.jpg)

A (very much) work-in-progress matching decompilation of the greatest N64 game ever made, [Snowboard Kids 2](https://en.wikipedia.org/wiki/Snowboard_Kids_2).

**This repository does not contain any game assets or assembly whatsoever. An existing copy of the game is required. This project is not a port, to PC or any other platform. It takes a Nintendo 64 rom, extracts assets from it, and combines them with C code we reverse-engineered from the rom, to produce an identical rom. It is explicitly a non-goal for this project to be used for any commercial purpose.**

## Dependencies

This project has been tested on Ubuntu (x86). Your milege may vary on other systems.

System packages:

* make
* git
* docker
* python3
* pip3
* binutils-mips-linux-gnu

Build tools:

```bash
make setup
```

Build Python dependencies:

```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -U -r requirements.txt
```

## Building

Copy your big-endian Snowboard Kids 2 rom into the root of the repository. Rename it to `snowboardkids2.z64`. Then run:

```bash
make clean
make extract
make
```

If everything works correctly you should see:

```bash
build/snowboardkids2.z64: OK
```

Acknowledgements
================
This project wouldn’t be possible without the collective knowledge, tools, and support of the broader decompilation community. Huge thanks to contributors of other N64 decomp projects, the teams behind [decomp.me](https://decomp.me/) and [decomp.dev](https://decomp.dev/), and the incredibly helpful discussions happening on Discord. These resources have been invaluable for solving problems, speeding up setup, and staying motivated throughout the process.
