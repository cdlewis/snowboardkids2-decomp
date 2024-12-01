![progress](https://img.shields.io/badge/progress-4.61%25-blue)

# Snowboard Kids 2

A (very much) work-in-progress matching decomp of the N64 game, Snowboard Kids 2.

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
