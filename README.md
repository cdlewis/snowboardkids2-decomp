![progress](https://img.shields.io/badge/progress-8.01%25-blue)

# Snowboard Kids 2

![box art](Snowboard_Kids_2_cover.jpg)

A (very much) work-in-progress matching decompilation of the greatest N64 game ever made, [Snowboard Kids 2](https://en.wikipedia.org/wiki/Snowboard_Kids_2).

## Dependencies

System packages (list is incomplete):

* make
* git
* docker
* python3
* pip3

Build GCC 2.7.2:

```bash
cd tools/old-gcc
VERSION=2.7.2 make
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
