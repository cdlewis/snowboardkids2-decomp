# Investigation: Merging 3D + 2D Rendering into a Single Task

## Context

A theory was proposed about `src/graphics/graphics.c`: the original SBK2 developers submit a separate RSP task per microcode (F3DEX2 for 3D, S2DEX for 2D). The claim is that they could have merged everything into a **single display list / single task** and used the GBI command `gSPLoadUcode` (a.k.a. `gsSPLoadUcodeL`) to swap microcodes mid-DL, eliminating redundant `gDPFullSync`s and per-task RSP boot/yield overhead.

This document captures both **what the code does today** and **the precise mechanics of the proposed merged-task design**, so the theory can be evaluated end-to-end.

---

## What the code does today

All references are in `src/graphics/graphics.c`.

### 1. Two microcodes registered and indexed by viewport priority

`graphics.c:221-224`:
```c
UcodeEntry microcodeGroups[] = {
    { gspS2DEX_fifoTextStart,  gspS2DEX_fifoDataStart  }, // [0] = 2D
    { gspF3DEX2_fifoTextStart, gspF3DEX2_fifoDataStart }, // [1] = 3D
};
```
Each viewport carries a `priority` (`u8`) used directly as the index into this array (`graphics.c:509-510`).

### 2. One `OSScTask` is built per priority group

In `renderFrame()` (`graphics.c:394-520`), the outer `do/while` walks the viewport list (`list3_next`). For each contiguous run of viewports sharing the same `priority`:

- Allocate a `frameCallbackMsg` (an `OSScTask`-like struct) — `graphics.c:468`.
- Build a tiny **wrapper DL** (`graphics.c:469-485`): `gSPSegment(0,0)`, two raw `0xE7000000` (`G_RDPPIPESYNC`), `gDPFullSync`, `gSPEndDisplayList`. Each priority-group's *real* DL is jumped into from the per-viewport setup loop further down.
- Populate `OSTask.ucode` / `ucode_data` from `microcodeGroups[priority]` (`graphics.c:509-510`); standard boot/yield/output/dram-stack buffers (`graphics.c:507-517`).

A scene of `[3D, 3D, 2D]` therefore produces **2 tasks** (one F3DEX2, one S2DEX), each ending with its own `gDPFullSync`.

### 3. Tasks are submitted one-by-one

`processDisplayFrameUpdate()` (`graphics.c:343-369`) walks the same list and calls `sendMessageToThreadSyncQueue()` once per `frameCallbackMsg`, then sends the framebuffer-init task `gDisplayBufferMsgs`.

### 4. `gSPLoadUcode` is never used

`grep` over `src/` and `include/` confirms `gSPLoadUcode` / `gsSPLoadUcodeL` exist only as macros in the ultralib GBI headers (`lib/ultralib/include/PR/gbi.h:2401-2411`). No project code emits the command. The only mechanism the codebase uses to switch microcodes is `OSTask.ucode` on a fresh task.

---

## How the proposed solution works (mechanics)

### The GBI command itself

From `lib/ultralib/include/PR/gbi.h:2383-2411`, `gsSPLoadUcode` expands to **two adjacent 64-bit Gfx commands**:

| word | opcode | payload |
|------|--------|---------|
| 1 | `G_RDPHALF_1` (`0xE1`) | `uc_dstart` (physical addr of new ucode's *data* segment) |
| 2 | `G_LOAD_UCODE` (`0xDD`) | `uc_start` (physical addr of new ucode's *text* segment), and `uc_dsize-1` in the lower 16 bits |

The convenience macro `gsSPLoadUcodeL(gspS2DEX_fifo)` resolves both pointers via `OS_K0_TO_PHYSICAL(&...TextStart)` / `...DataStart`, with `uc_dsize = SP_UCODE_DATA_SIZE` (0x800 — same size the existing tasks use at `graphics.c:511`).

### What the RSP does when it sees `G_LOAD_UCODE`

The command is implemented by the **RSP boot loader stub** (`rspboot`, the same code that runs at task start). When the running microcode (F3DEX2 or S2DEX) hits `G_LOAD_UCODE` in its command stream, it jumps back into the boot stub, which:

1. DMAs `uc_dsize` bytes (0x800) of the new microcode's data segment from `uc_dstart` into DMEM.
2. DMAs the new microcode's text segment from `uc_start` into IMEM.
3. Resumes processing the GBI command stream from the next `Gfx` word, but now executing under the new microcode.

Crucially:
- **DMEM is fully reinitialized** from the new ucode's data segment. Matrix stack, segment table, lights, viewport, scissor, geometry mode, combine state — everything the RSP tracks — is reset to that ucode's defaults. The current per-task design has the same property (each task starts with fresh DMEM), so behavior parity is preserved.
- **The RDP is untouched.** `G_LOAD_UCODE` only swaps RSP code/data; the RDP's pipeline, framebuffer pointer, fill color, scissor, and any in-flight commands keep going. This is why a `gDPFullSync` between groups is **not** needed for correctness — the RDP doesn't care which RSP microcode is feeding it.

### What the merged single-task DL looks like

OSTask is set up *once*, with the **first** microcode that will run as `OSTask.ucode` (e.g. F3DEX2) — that bootstraps the RSP. Buffers (`dram_stack`, `output_buff`, `yield_data`) are shared across the whole frame, identical to today.

For a scene `[3D group A, 2D group B, 3D group C]`, the merged DL becomes:

```
; --- Boot: OSTask.ucode = F3DEX2 ---
gSPSegment(0, 0)
< group A: 3D viewport setup + draws >
gDPPipeSync                              ; flush RDP attribute changes (cheap)
gsSPLoadUcodeL(gspS2DEX_fifo)            ; switch RSP to S2DEX
gSPSegment(0, 0)                         ; re-establish segment table for S2DEX
< group B: 2D viewport setup + draws >
gDPPipeSync
gsSPLoadUcodeL(gspF3DEX2_fifo)           ; switch back to F3DEX2
gSPSegment(0, 0)
< group C: 3D viewport setup + draws >
gDPFullSync                              ; ONE full-sync for the whole frame
gSPEndDisplayList
```

Submitted as **one** `osScKickTask`. The work that disappears versus today:

| Today (per group) | Merged (per group) |
|---|---|
| `osScKickTask` enqueue + scheduler dispatch | none after the first |
| RSP boot ucode runs (`rspbootTextStart`→`aspMainTextStart` DMA) | runs once at frame start |
| Yield save / restore at task boundary | none between groups |
| `gDPFullSync` + DP interrupt + RDP pipeline drain | only the final one |

The wrapper DL's per-group `gSPSegment(0,0)` and pipe-syncs (`graphics.c:471, 475-481`) **are still needed** at each boundary — they're per-microcode state init, not per-task overhead. The build loop in `renderFrame` would just emit them inline into one shared `gRegionAllocPtr` stream instead of into N separate wrapper DLs, with `gsSPLoadUcodeL` replacing the task boundary.

### Concrete code-level shape of the change

A faithful sketch (non-matching by definition):

- `processDisplayFrameUpdate` collapses to: send **one** message (the merged-frame task) plus the existing `gDisplayBufferMsgs` init message. The per-viewport `sendMessageToThreadSyncQueue` loop at `graphics.c:354-360` goes away.
- `renderFrame` keeps the priority-grouping walk at `graphics.c:456-519`, but instead of allocating a separate `frameCallbackMsg` per group, it appends `gsSPLoadUcodeL(...)` into a single shared DL whenever `priority` changes from the previous group. Only the final group emits `gDPFullSync` + `gSPEndDisplayList`. A single `frameCallbackMsg` (probably hung off `gRootViewport`) holds the combined task.
- `OSTask.ucode` / `ucode_data` for the merged task = the *first* group's microcode. Everything else (`ucode_data_size = 0x800`, `dram_stack`, `output_buff`, `yield_data`) is unchanged.

---

## Caveats worth understanding

1. **Yield/scheduling semantics change.** Each task currently has its own `scanlineValue` (`graphics.c:501`) and `taskFlags` (`graphics.c:488,495`). Those control when the RSP can yield to audio. Merging into one task means the RSP can no longer yield at group boundaries — only at intra-microcode yield points. If audio underruns appear, the merged task's yield bit / scanline setup needs tuning. (In practice, audio yields against the RSP scheduler regardless, so this is usually fine.)
2. **`output_buff` reuse.** All tasks share `gOutputBuffer` of size `BUFFER_SIZE` (`graphics.c:514-515`). The merged DL's RDP output must still fit in that buffer over the *whole frame*; today it just had to fit per-group. In practice it's the same total bytes, but if any task currently relies on the buffer being "reset" between groups, that assumption breaks. Worth a `BUFFER_SIZE` audit before any rewrite.
3. **`gSPLoadUcode` requires the *fifo* (non-boot) variant of each microcode.** The codebase already stores `gspS2DEX_fifoTextStart` / `gspF3DEX2_fifoTextStart` (`graphics.c:222-223`), which is exactly the right symbol for `gsSPLoadUcodeL`. No new linkage or new ucode binaries needed.
4. **Decomp impact.** This is a *matching* decomp project. Restructuring `renderFrame` / `processDisplayFrameUpdate` into a single-task design produces output that does not match the original ROM. It's not appropriate for `main`; it belongs on a separate "improvements" / hack branch built on top of a matched baseline.

---

## Conclusion

Theory verified, mechanics fully understood:

- **Today:** N tasks per frame (one per priority group). Each pays RSP boot + scheduler dispatch + yield save/restore + a closing `gDPFullSync`. The microcode swap is implicit in `OSTask.ucode`.
- **Proposed:** 1 task per frame. Microcode swaps happen in-stream via `gsSPLoadUcodeL`, which the RSP boot stub services without OS involvement. Only the last command in the frame's DL needs `gDPFullSync` because `G_LOAD_UCODE` doesn't disturb the RDP. Per-microcode state init (segment table, etc.) is still emitted at each boundary, but inline.

Any actual rewrite must live outside the matching effort.

## Critical files referenced

- `src/graphics/graphics.c:221-224` — microcode table
- `src/graphics/graphics.c:343-369` — `processDisplayFrameUpdate` (per-task submission loop)
- `src/graphics/graphics.c:456-519` — per-priority task-build loop (incl. wrapper DL + `gDPFullSync` at 483)
- `lib/ultralib/include/PR/gbi.h:2377-2411` — `gSPLoadUcode` / `gsSPLoadUcodeL` macros (`G_RDPHALF_1` + `G_LOAD_UCODE` pair)
- `lib/ultralib/include/PR/gbi.h:102, 155` — `G_LOAD_UCODE` opcode (`0xDD`) and FIFO-build variant
