---
name: convert-asset-array
description: Convert arrays with hardcoded pointer addresses to use ROM_START/ROM_END symbols. This automates the process of adding USE_ASSET declarations, updating array initialization, and configuring YAML segments.
---

# Convert Asset Arrays to ROM Symbols

## Overview

When working with data arrays that contain hardcoded ROM pointer addresses (like `gCourseDataAssets` or `gSpriteAssets`), you need to:
1. Add `USE_ASSET` declarations for each unique address
2. Convert the array to use `ROM_START`/`ROM_END` symbols
3. Update `snowboardkids2.yaml` to add named segments with proper end markers

This skill automates that entire process.

## When to use

Use this skill when you see arrays with hardcoded pointer addresses like:

```c
Asset_56910 gCourseDataAssets[] = {
    { (void *)0x00224420, (void *)0x00226BA0, 0x000024A8 },
    { (void *)0x0023CB80, (void *)0x00240520, 0x00003468 },
    // ...
};
```

These should be converted to:

```c
Asset_56910 gCourseDataAssets[] = {
    { (void *)&_224420_ROM_START, (void *)&_224420_ROM_END, 0x000024A8 },
    { (void *)&_23CB80_ROM_START, (void *)&_23CB80_ROM_END, 0x00003468 },
    // ...
};
```

## How to use

### Identify the array to convert

Look for arrays of type `Asset_56910` or similar asset structures in `src/data/course_data.c` or other data files.

### Extract the pattern

For each array entry, extract:
- Start address (first pointer value)
- End address (second pointer value)
- Size field (third value, typically ignored for ROM_START/ROM_END)

### Step 1: Add USE_ASSET declarations

In `include/assets.h`, add USE_ASSET declarations for each unique start address:

```c
USE_ASSET(_224420);
USE_ASSET(_23CB80);
USE_ASSET(_24D6C0);
// ... etc, sorted numerically
```

### Step 2: Update the array initialization

Convert the array entries from:
```c
{ (void *)0x00224420, (void *)0x00226BA0, 0x000024A8 }
```

To:
```c
{ (void *)&_224420_ROM_START, (void *)&_224420_ROM_END, 0x000024A8 }
```

The pattern is:
- Remove leading `0x` and `00` prefixes from addresses
- Replace `(void *)0xADDRESS` with `(void *)&_ADDRESS_ROM_START` or `_ROM_END`
- Keep the size field unchanged

### Step 3: Update snowboardkids2.yaml

For each asset segment:

1. **Find the location** - Search for the address in the YAML:
   ```bash
   grep -n "0x224420\|0x226BA0" snowboardkids2.yaml
   ```

2. **Add or convert segments** - If you see:
   - `[0x224420, bin]` alone → Convert to named segment:
     ```yaml
     - name: _224420
       type: bin
       vram: 0
       start: 0x224420
     ```

   - Named segment without end marker → Add end marker:
     ```yaml
     - name: _224420
       type: bin
       vram: 0
       start: 0x224420

     - [0x226BA0, bin]  # This marks where _224420 ENDS
     ```

3. **Maintain proper ordering** - Ensure segments are in ascending address order:
   - Named segment definition
   - End marker (next segment's start)
   - Next named segment

Example structure:
```yaml
- name: _224420
  type: bin
  vram: 0
  start: 0x224420

- [0x226BA0, bin]  # End marker for _224420, also marks start of next

- name: _23CB80
  type: bin
  vram: 0
  start: 0x23CB80

- [0x240520, bin]  # End marker for _23CB80
```

### Step 4: Validate

Build and verify:
```bash
./tools/build-and-verify.sh
```

Watch for errors:
- **Segment XXXXX has zero size** - End marker followed by named segment at same address (remove redundant end marker)
- **Segments out of order** - YAML entries not in ascending address order
- **Missing USE_ASSET** - Undefined ROM_START/ROM_END symbols
- **Wrong segment size** - End marker at wrong address causing data mismatch
- **Checksum mismatch** - Use `data_diff.py --find-first-mismatch` to find the problematic symbol

## Common Patterns

### Pattern 1: Single address used by multiple entries

**Why**: Multiple array entries may reference the same asset (duplicates share data).

### Pattern 2: Asset marker already exists as [0xADDRESS, bin]

If multiple array entries use the same start/end addresses (common for duplicate assets), only declare the asset once:

```c
// Both entries 15 and 16 use the same addresses
{ (void *)&_34A2C0_ROM_START, (void *)&_34A2C0_ROM_END, 0x00000438 },
{ (void *)&_34A2C0_ROM_START, (void *)&_34A2C0_ROM_END, 0x00000438 },
```

### Pattern 2: Asset marker already exists as [0xADDRESS, bin]

When you see `[0x226BA0, bin]` in the YAML:
1. Add a named segment BEFORE it:
   ```yaml
   - name: _224420
     type: bin
     vram: 0
     start: 0x224420

   - [0x226BA0, bin]  # End of _224420
   ```

2. Convert the marker to a named segment with its own end marker:
   ```yaml
   - name: _226BA0
     type: bin
     vram: 0
     start: 0x226BA0

   - [0x228400, bin]  # End of _226BA0
   ```

### Pattern 3: Redundant end markers

**WRONG**: End marker followed by named segment at same address:
```yaml
- name: _226BA0
  type: bin
  vram: 0
  start: 0x226BA0

- [0x228400, bin]        # REDUNDANT! _228400 starts at same address

- name: _228400
  type: bin
  vram: 0
  start: 0x228400
```

**CORRECT**: Named segment serves as boundary:
```yaml
- name: _226BA0
  type: bin
  vram: 0
  start: 0x226BA0

- name: _228400          # Ends _226BA0, starts at 0x228400
  type: bin
  vram: 0
  start: 0x228400
```

**When explicit end markers ARE needed**:
- When there's a gap before the next named segment
- When the next "segment" is just an address marker without data
- When you need precise control over where a USE_ASSET segment ends

## Quick Reference Commands

```bash
# Check if asset exists
grep "_224420:" snowboardkids2.yaml

# Find all uses of an address
grep -r "0x00224420" src/

# Validate segment order
python3 -m splat split snowboardkids2.yaml

# Build and verify
./tools/build-and-verify.sh

# Check data mismatches
python3 tools/data-differ/data_diff.py gSpriteAssets

# Find first mismatching data symbol
python3 tools/data-differ/data_diff.py --find-first-mismatch
```

## Debugging and Troubleshooting

### Zero-size segment errors

**Error**: `Segment 228400 has zero size.`

**Cause**: Having both an end marker `[0xADDRESS, bin]` AND a named segment starting at the same address creates a zero-size segment.

```yaml
# WRONG - Creates zero-size segment:
- [0x228400, bin]        # End marker for _226BA0

- name: _228400          # Named segment at SAME address
  type: bin
  vram: 0
  start: 0x228400
```

**Fix**: Remove the redundant end marker when a named segment exists at that address:

```yaml
# CORRECT - Named segment serves as boundary:
- name: _228400
  type: bin
  vram: 0
  start: 0x228400
```

**Why**: A named segment automatically marks the end of the previous segment. The previous segment (_226BA0 in this case) ends where _228400 starts.

### Understanding segment boundaries

**Key principle**: Named segments automatically end where the next segment starts.

```yaml
- name: _226BA0
  type: bin
  vram: 0
  start: 0x226BA0
  # Automatically ends at 0x228400 (where _228400 starts)

- name: _228400
  type: bin
  vram: 0
  start: 0x228400
  # Automatically ends at 0x23CB80 (where _23CB80 starts)

- name: _23CB80
  type: bin
  vram: 0
  start: 0x23CB80
```

**When to use explicit end markers**:
- When there's a gap between segments (unused ROM space)
- When you need to end a segment at an address that doesn't have a named segment
- When a segment is used but doesn't need a named segment in the YAML

```yaml
# Example with explicit end marker:
- name: _34A2C0
  type: bin
  vram: 0
  start: 0x34A2C0

- [0x34A750, bin]      # Explicit end marker - _34A2C0 ends here

- name: _34CB50        # Gap from 0x34A750 to 0x34CB50
  type: bin
  vram: 0
  start: 0x34CB50
```

### Finding all redundant end markers

When debugging zero-size segment errors, use this Python script to find all redundant end markers:

```python
import re

with open('snowboardkids2.yaml', 'r') as f:
    lines = f.readlines()

for i in range(len(lines) - 1):
    end_match = re.match(r'\s*-\s*\[0x([0-9A-Fa-f]+),\s*bin\]', lines[i])
    if end_match:
        end_addr = end_match.group(1).upper()
        for j in range(i+1, min(i+5, len(lines))):
            name_match = re.match(r'\s*-\s*name:\s*_' + end_addr + r'\b', lines[j])
            if name_match:
                print(f"Line {i+1}: [0x{end_addr}, bin] followed by name: _{end_addr} at line {j+1}")
                break
```

### Common YAML structure mistakes

1. **End marker followed by named segment at same address** → Remove end marker
2. **Named segment with no end marker when needed** → Add `[0xEND_ADDRESS, bin]`
3. **Segments out of ascending address order** → Reorder segments
4. **Missing USE_ASSET declaration** → Add to `include/assets.h`

### Only create named segments for addresses used with ROM symbols

Before adding a named segment to the YAML, verify the address is actually used with ROM_START/ROM_END symbols:

```bash
# Check if address is used with ROM symbols
grep -r "_228400_ROM" src/ include/
```

If the address is only used as a hardcoded pointer (e.g., `{ (void *)0x00228400, ... }`), you typically don't need a named segment. An end marker is sufficient:

```yaml
# If _228400 is only used as hardcoded pointer:
- [0x228400, bin]        # Just marks end of previous segment

# If _228400 is used with ROM symbols:
- name: _228400          # Needed for USE_ASSET(_228400)
  type: bin
  vram: 0
  start: 0x228400
```

**Exception**: You may need a named segment even if not currently using ROM symbols, if you plan to convert the code later.

### Data mismatch debugging

If the build fails with checksum mismatch:

1. Find the first mismatching symbol:
   ```bash
   python3 tools/data-differ/data_diff.py --find-first-mismatch
   ```

2. Examine the byte differences to understand which addresses are wrong

3. Check the YAML segments around those addresses:
   ```bash
   grep -n "0xADDRESS" snowboardkids2.yaml
   ```

4. Verify:
   - Named segments exist for all used addresses
   - End markers are at correct addresses
   - No zero-size segments
   - Segments are in ascending order

## Example: Converting gSpriteAssets

Original array:
```c
Asset_56910 gSpriteAssets[] = {
    { (void *)0x00226BA0, (void *)0x00228400, 0x00002208 },
    { (void *)0x00240520, (void *)0x002415B0, 0x00001BA8 },
    // ... 14 more entries
};
```

After conversion:
```c
Asset_56910 gSpriteAssets[] = {
    { (void *)&_226BA0_ROM_START, (void *)&_226BA0_ROM_END, 0x00002208 },
    { (void *)&_240520_ROM_START, (void *)&_240520_ROM_END, 0x00001BA8 },
    // ... 14 more entries
};
```

With YAML segments:
```yaml
- name: _226BA0
  type: bin
  vram: 0
  start: 0x226BA0

- [0x228400, bin]

- name: _240520
  type: bin
  vram: 0
  start: 0x240520

- [0x2415B0, bin]
```
