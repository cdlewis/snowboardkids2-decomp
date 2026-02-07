---
name: add-asset-to-yaml
description: Adding a new asset to snowboardkids2.yaml requires following specific steps to ensure assets are in the correct order and properly declared. Use this skill when you need to add a new binary asset.
---

# Add a new asset to snowboardkids2.yaml

## Overview

Binary assets (data blobs extracted from the ROM) must be defined in `snowboardkids2.yaml` before they can be used in C code via the `USE_ASSET()` macro. This skill documents how to identify when an asset needs to be added and how to add it correctly.

## When to add an asset

You need to add an asset when you see code that uses raw ROM addresses directly, such as:

```c
animationDataStart = (void *)0x0045B130,
animationDataEnd = (void *)0x0045D620,
```

These should be converted to use asset references:

```c
animationDataStart = (void *)&_45B130_ROM_START,
animationDataEnd = (void *)&_45B130_ROM_END,
```

## Steps to add an asset

### 1. Check if the asset already exists

Search `snowboardkids2.yaml` for the asset name. Asset names follow the pattern `_<hex_address>`:

```bash
grep "_45B130:" snowboardkids2.yaml
```

The asset may exist as:
- A named asset:
  ```yaml
  - name: _45B130
    type: bin
    vram: 0
    start: 0x45B130
  ```
- An unnamed bin entry:
  ```yaml
  - [0x45B130, bin]
  ```

### 2. Add or convert the asset in snowboardkids2.yaml

#### If it's an unnamed bin entry:
Convert it to a named asset:
```yaml
- name: _45B130
  type: bin
  vram: 0
  start: 0x45B130
```

#### If the asset doesn't exist:
Add it in **numerical order** with other assets at the same address range. This is critical - the yaml must be in ascending order by start address.

For example, when adding `_45D620` after `_45B130`:
```yaml
  - name: _45B130
    type: bin
    vram: 0
    start: 0x45B130

  - name: _45D620    # Must come AFTER _45B130 (0x45B130 < 0x45D620)
    type: bin
    vram: 0
    start: 0x45D620
```

**Common mistake**: Adding assets out of order will cause a build error:
```
Error: segments out of order - (_5AD910 starts at 0x5AD910, but next segment starts at 0x5AD1C0)
```

To fix this, ensure assets are sorted by their start address (hexadecimal numerically).

### 3. Add USE_ASSET declaration

In the C file that uses the asset, add a `USE_ASSET()` declaration after the includes and before any code:

```c
#include "20F0.h"

USE_ASSET(_45B130);
USE_ASSET(_45D620);
// ... more assets ...

typedef struct {
    // ...
} SomeStruct;
```

The `USE_ASSET` declarations should be sorted numerically by address for consistency.

### 4. Update code to use asset references

Replace raw addresses with the asset's `ROM_START` and `ROM_END` symbols:

**Before:**
```c
.animationDataStart = (void *)0x0045B130,
.animationDataEnd = (void *)0x0045D620,
```

**After:**
```c
.animationDataStart = (void *)&_45B130_ROM_START,
.animationDataEnd = (void *)&_45B130_ROM_END,
```

The pattern is:
- Start address → `_<ADDRESS>_ROM_START`
- End address → `_<ADDRESS>_ROM_END`

Note: The `_ROM_END` symbol points to the end of the asset as defined in the yaml, not necessarily the end address used in the original code. The `animationDataSize` field or actual code logic determines how much of the asset is used.

## Validation

After adding assets, verify the build succeeds:

```bash
./tools/build-and-verify.sh
```

Common errors to watch for:
- **Segments out of order**: Assets are not in numerical order by start address
- **Undefined reference**: Missing `USE_ASSET()` declaration
- **Asset not found**: Asset definition missing from yaml

## Example: Adding multiple assets

When adding multiple related assets (e.g., a series of animation data blocks), ensure:

1. All assets are added to yaml in numerical order
2. All assets have `USE_ASSET()` declarations
3. All address pairs in the code are converted to use `ROM_START/ROM_END`

Example batch of assets:
```yaml
  - name: _4ED9C0
    type: bin
    vram: 0
    start: 0x4ED9C0

  - name: _4F45E0
    type: bin
    vram: 0
    start: 0x4F45E0

  - name: _4FDE20
    type: bin
    vram: 0
    start: 0x4FDE20

  # ... and so on in ascending order
```

## Tips

- Use `grep` to find all instances of a raw address pattern: `grep -r "0x0045B130" src/`
- When converting many addresses, consider using `sed` for batch replacements
- Always verify the numerical order in yaml - hexadecimal values can be tricky to sort manually
- Null pointers (`0x00000000`) should not be converted to assets
