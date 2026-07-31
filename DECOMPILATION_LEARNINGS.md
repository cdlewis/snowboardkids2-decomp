# Decompilation Learnings

Decompilation Learnings is a record maintained by Claude of insights from matching functions in this project. These document compiler behavior, codegen quirks, and patterns specific to KMC GCC 2.7.2 with `-O2 -mips3`.

## Reusing UI Render Argument Types

Menu task records passed to `renderTextSprite`, `renderAlphaBlendedTextSprite`, or `renderTextLayout` are often
complete `TextRenderArg` or `TextLayoutArg` values, even when a local struct gives their fields menu-specific names.
Reusing the renderer types identifies byte 0x0C of `TextRenderArg` as `tileMode`, byte 0x0D as
`overridePaletteCount`, and its 16-bit color union as the palette/alpha pair. On big-endian N64, assigning a pulse
value to `color.paletteAndAlpha` places that value in the low alpha byte.

Task records can also contain adjacent canonical render arguments even when their local fields appear unrelated.
The character-select coordinate debugger stores two complete `TextData` values after its selected coordinate:
one renders the formatted coordinate label and the other renders the movable marker. Likewise, a loaded tile-map
asset kept immediately after a `TileMapScrollRenderState` is task-owned data, not padding in a new tile-map type;
represent it as a small containing task state with the common renderer state as its first member.

`ViewportNode` is 0x1D8 bytes and can be embedded at the start of menu allocation state structs. This replaces
0x1D8-byte padding arrays while keeping subsequent task fields at their original offsets.

When iterating adjacent named renderer fields from the first field's typed address, KMC may fold the field offset
into the base pointer and shorten the function. Keeping the containing struct pointer live with an empty inline-asm
input can retain the target's base-plus-field-offset address calculation without reverting to raw byte offsets.

## Reusing `DisplayListObject` in Task Layouts

Level task states often embed one or more complete `DisplayListObject` values even when a function only accesses
the transform, display-list pointer, or asset segments. Prefer embedding the shared type over creating separate
partial views for initialization, rendering, and cleanup. Since `DisplayListObject` is 0x3C bytes, adjacent objects
commonly begin at offsets 0x00 and 0x3C; using the shared type preserves the original field offsets and codegen.

When a 16-bit task timer is also read as an 8-bit alpha value on big-endian N64, a union provides both semantic
views without a local offset-based alias struct. The low byte of the `s16` is at the second byte of the union.

## Nested If/Else Block Layout

GCC 2.7.2 lays out nested if/else blocks in a predictable order:

- **Outer condition** is checked first (generates the first `beq`/`bne`)
- **Inner else** block is placed before the **outer else** block in memory
- The **fall-through** path (both conditions true for `!=` checks) comes first

This means you **cannot independently control** check order and block layout with standard nested ifs:

| Structure | Check Order | Block Layout |
|-----------|------------|--------------|
| `if (!=A) { if (!=B) {..} else {B} } else {A}` | A first | fall-through → B(inner) → A(outer) |
| `if (!=B) { if (!=A) {..} else {A} } else {B}` | B first | fall-through → A(inner) → B(outer) |

### Switch Statements Solve This

When you need **both** a specific check order **and** a specific block layout that nested ifs can't provide, use a `switch` statement with `default:` first:

```c
switch (value) {
default:
    // fall-through code (placed first, no jump needed)
    break;
case 1:
    // checked first, placed after default
    break;
case 8:
    // checked second, placed after case 1
    break;
}
```

For sparse cases (e.g., 1 and 8), GCC 2.7.2 generates sequential `beq` comparisons (not a jump table). Cases are checked and laid out in source order, and `default:` first makes it the fall-through path — no extra `j`/`nop` needed.

## Duplicated Function Calls vs Shared Calls

When the same function is called in both branches of an if/else with only a constant argument differing, **duplicate the call** rather than using a shared variable:

```c
// WRONG: shared call with variable — compiler may place move a0,reg in shared code
if (condition) {
    arg1 = 2;
} else {
    arg1 = 1;
}
setPlayerLeanAnimation(arg0, arg1, value);

// CORRECT: duplicated calls — compiler places move a0,reg in each branch
if (condition) {
    setPlayerLeanAnimation(arg0, 2, value);
} else {
    setPlayerLeanAnimation(arg0, 1, value);
}
```

The shared-variable approach generates one copy of `move a0,s2` in shared code, while the target may have it duplicated per-branch. This causes the function to be 4 bytes shorter, cascading to all branch offsets being wrong.

## Block-Scoped Variables for Register Control

Block-scoped variables in C89 can get different register allocation than function-scope variables:

- A function-scope `s16` variable may be assigned a callee-saved register (e.g., `s3`), persisting across function calls
- A block-scoped `s16` variable is more likely to use a temporary register (e.g., `v1`), matching the target when the value doesn't need to survive across calls
- Block-scoped variables also avoid changing the stack frame size (KMC assembler does dead stack slot elimination)

## Stack Frame Size (KMC Assembler)

- The KMC assembler eliminates unreferenced stack slots — unused local variables don't contribute to the frame
- `volatile` padding arrays DO work to force stack size if placed correctly
- Declaration order affects stack layout: earlier declarations get lower `sp` offsets
- Adding/removing a single `s32` variable can change the frame by 8 bytes (due to alignment)
- Use `s32 pad[N]` arrays to fine-tune the frame size; adjust N as needed

## Signed Division

Write natural division (`x / 2`, `x / 4`) and let the compiler emit the shift-and-bias pattern for signed integers. Don't manually write the shift pattern — it won't match.

## Branch-Likely (bnel/beql) Generation

GCC 2.7.2 with `-mips3` can generate branch-likely instructions (`bnel`, `beql`). The if/else branch order in C affects whether the compiler uses `bnel` vs `beq+j`:

```c
// Generates bnel (branch not equal likely):
if (scaleS == -1) {
    clipOffsetX -= delta;  // else-like path first
} else {
    clipOffsetX = delta;   // simple assignment second
}

// Generates beq+j (less efficient):
if (scaleS != -1) {
    clipOffsetX = delta;   // simple assignment first
} else {
    clipOffsetX -= delta;  // else-like path second
}
```

When the "true" condition's body is a single instruction that fits in a delay slot, putting it as the `else` branch (with the inverted condition as the `if`) triggers `bnel`. The compiler puts the simple assignment in the branch-likely delay slot (annulled if not taken).

## `beqzl` vs `beqz`: `for` Loop `continue` vs `while` Loop `goto`

GCC 2.7.2's delay slot filler prefers `beqzl` (branch-likely) over `beqz` when the branch target provides a good delay slot candidate. In a `for` loop, `continue` branches to the loop increment (`addiu t0,t0,1`), which is always eligible for `beqzl`'s delay slot. This means **`continue` in a `for` loop always generates `beqzl`**.

To get `beqz` (regular branch) with the fall-through instruction in the delay slot, convert the `for` loop to a `while` loop with an explicit `goto` to a label at the bottom and a manual increment:

```c
// Generates beqzl (branch-likely) — for loop + continue
for (i = 0; (u16)i < 4; i++) {
    if (!condition) continue;  // beqzl with i++ in delay slot
    // body
}

// Generates beqz (regular) — while loop + goto + explicit i++
i = 0;
while ((u16)i < 4) {
    if (!condition) goto next;  // beqz with fall-through in delay slot
    // body
    next:
    i++;
}
```

The `while` loop's `goto next` targets a user-defined label, not the loop's continue label. GCC's delay slot filler doesn't find the increment as a beqzl candidate at the goto target, so it falls back to using the fall-through instruction in a regular `beqz` delay slot.

## Register Allocation: Extra Variables Affect Register Choice

Adding an explicit local variable for a subexpression can change which physical register the compiler assigns to other variables. For example:

```c
// Without: frameEntry ends up in $a3, width reloaded from memory
scaleW = (frameEntry->width << 12) / arg0->renderWidth;
clipOffsetX = frameEntry->width * 4 - 4;  // re-accesses frameEntry->width

// With: frameEntry moves to $t0, widthTimes4 occupies $a3
s32 widthTimes4 = frameEntry->width << 2;
scaleW = (frameEntry->width << 12) / arg0->renderWidth;
clipOffsetX = widthTimes4 - 4;  // uses cached value
```

The extra variable creates additional register pressure that shifts the graph coloring, potentially moving a long-lived pointer from an argument register ($a3) to a temp register ($t0).

## `addu` Operand Order in Delay Slots

When computing a struct array element address for a function call argument (e.g., `memcpy`), the `addu` operand order depends on the C expression form:

- `&array[i].field` or `array[i].transformationMatrix` → `addu rd, base, offset` (base register first)
- `(void *)(i * (s32)sizeof(Struct) + (s32)ptr)` → `addu rd, offset, base` (offset first)

When the field is at offset 0 within the struct (e.g., `transformationMatrix` at the start), the compiler simplifies away the field offset and normalizes operand order. Using explicit integer arithmetic with the offset expression on the left preserves the original operand ordering:

```c
// Generates: addu a0, a0, s0 (base first — WRONG for some targets)
memcpy(i[ent->unk00].transformationMatrix, &identityMatrix, 0x20);

// Generates: addu a0, s0, a0 (offset first — matches target)
memcpy((void *)(i * (s32)sizeof(AssetSlot) + (s32)ent->unk00), &identityMatrix, 0x20);
```

Note: `i[array]` (equivalent to `array[i]`) flips the expression tree, which affects operand order for field accesses at non-zero offsets but not at offset 0.

## Byte-Level Access to Word Fields (Overlay Struct)

When the target assembly uses `lb`/`lbu` (byte loads) to access individual bytes of what is stored as an `s32` in the data section, use an overlay struct cast instead of splitting the `s32` into sub-word fields:

```c
// Overlay struct for byte-level access
typedef struct {
    u8 _pad[0x30];
    s8 signedByte;   // lb at offset 0x30
    u8 unsignedByte; // lbu at offset 0x31
} StructByteView;

// Access via cast — generates lb/lbu as needed
#define overlay ((StructByteView *)ptr)
if (overlay->signedByte == -1) { ... }
value = overlay->unsignedByte;
```

**Why not split the s32 field?** KMC GCC 2.7.2 has a bug/quirk where splitting an `s32` into `s8 + u8 + u8[2]` in a struct with designated initializers causes the data section to grow (32 bytes in the observed case). The overlay cast approach avoids modifying the struct definition or data initializers while generating the correct byte-load instructions.

## Register Allocation with `register __asm__` for Multiple Variables

When GCC 2.7.2 needs to load multiple constants into specific registers before a loop, the register assignment in C must use `register ... __asm__("$N")` directives. The declaration order determines the load order in the generated assembly — declare variables in descending register order (e.g., `$22`, `$21`, `$20`, `$19`, `$18`) to get them loaded in that order.

## Jump Table: Empty Cases vs Identical Case Bodies

When a switch statement has two consecutive cases with identical bodies, GCC 2.7.2 merges them — both jump table entries point to the same code address. If the target ROM has one case pointing to the post-switch code instead, that case was originally empty (`case N: break;`). Check the jump table `.rodata` section to distinguish merged cases from empty ones.

## Instruction Scheduling: Register Setup as Stall Fillers

GCC 2.7.2 -O2 fills load-use stall slots with independent instructions. When sequential memory operations (load-add-store for each struct field) are followed by a loop, the compiler interleaves loop-setup instructions into the stall slots between each load and its dependent add.

To control which instructions fill which stall slots, use `__asm__("")` barriers to create separate scheduling regions and place the setup instructions in each region:

```c
// Region 1: x offset + rotMatrix address computation
arg0->offset.x += global_x;
rm = &rotMatrix;              // fills x's load-use stall
__asm__("");
// Region 2: y offset + transformed address + constant
arg0->offset.y += global_y;
tf = &transformed;            // fills y's stall slot 1
yOffset = 0x80000;            // fills y's stall slot 2
__asm__("");
// Region 3: z offset + loop variable setup
arg0->offset.z += global_z;
particle = arg0->data;        // fills z's stall slot 1
rotation = 0;                 // fills z's stall slot 2
```

Each `__asm__("")` barrier creates a scheduling boundary. The scheduler fills stalls within each region using only the instructions in that region. Use `register __asm__("$N")` on the setup variables to ensure they go to the correct registers.

**Without barriers**: the compiler batches all loads together, then all adds, then all stores (no sequential pattern).
**With volatile struct fields**: forces sequential access but the scheduler picks its own fill order based on internal priorities — `register __asm__` constraints affect which instructions are prioritized first.

## Branchless Codegen for Simple Conditionals

GCC 2.7.2 -O2 generates branchless `sltu`/`negu`/`andi`/`ori` sequences for patterns like `x = 3; if (cond == 0) x = 2;` when using a local variable. To force a branch instead:

**Use direct struct field stores with if/else:**
```c
// WRONG: branchless codegen (sltu/negu/andi/ori)
numOptions = 3;
if (gameMode == 0) {
    numOptions = 2;
}
state->field = numOptions;

// CORRECT: generates bnez with delay slot fill
if (gameMode != 0) {
    state->field = 3;
} else {
    state->field = 2;
}
```

The if/else with direct stores generates a proper branch because the compiler treats each store as a separate side effect. The local-variable version allows the compiler to compute the value branchlessly since it's just a register operation.

Note: `__asm__("" : "=r"(var) : "0"(var))` barriers can prevent branchless codegen but cause `beqzl` (branch-likely) instead of `bnez`, and may allocate the wrong register.

## Duff's Device / Switch Fallthrough

GCC 2.7.2 supports Duff's device-style switch fallthrough. A `case` label inside an `else` block is valid C and generates the expected assembly:

```c
case 1:
    if (condition) {
        // ...
    } else {
        // ...
case 2:
        // shared code for case 1 (else) and case 2
    }
```

## Fixed Register Locals with Host Clang Checks

When a match needs a local variable assigned to a specific MIPS temporary register, KMC GCC accepts named registers such as `__asm__("t3")`. Host-side clang syntax checks may reject MIPS register names, so hide the register binding behind a clang-only empty macro:

```c
#ifdef __clang__
#define MIPS_REG_T3
#else
#define MIPS_REG_T3 __asm__("t3")
#endif

register s32 j MIPS_REG_T3;
```

For `updateAndRenderRaceCharacters`, this kept the inner ranking-loop index in `$t3`. A small overlay struct with the ranking entry at offset `0x64` then allowed field-style accesses while preserving the target `t2/t1/t0/a3` allocation.

## Forcing Hardware `mult` Over Synthetic Multiply (synth_mult)

GCC 2.7.2 decomposes multiply-by-constant into shift/add sequences (synth_mult) when the constant is known at compile time. If the target uses a hardware `mult` instruction, you need to hide the constant from the synth_mult pass.

**Key insight**: synth_mult runs *before* constant propagation. So if you compute the constant via a variable expression, synth_mult sees a variable multiply (uses `mult`) but later passes constant-propagate the value and emit `li` for the constant.

```c
// WRONG: literal constant → synth_mult decomposes into 7-instruction shift/add sequence
src = base + frameIndex * 0x1ED0;

// CORRECT: variable computed from another variable → hardware mult
s32 copySize = 0x7B4;
s32 stride = copySize * 4;  // compiler will propagate to 0x1ED0
__asm__("" : "=r"(stride) : "0"(stride));  // prevent further propagation
src = base + frameIndex * stride;  // generates: li v0, 0x1ED0; mult v1, v0; mflo v1
```

The `__asm__` barrier after computing `stride` prevents the compiler from propagating `stride` back into a constant for subsequent uses. Without the barrier, the compiler may re-derive the constant and use synth_mult anyway.

## Instruction Scheduling: Statement Ordering Controls `andi`/`lw`/`sll` Order

GCC 2.7.2 -O2 schedules instructions based on evaluation order of C statements. When computing `idx * sizeof(Struct)` where `idx` comes from a mask and a pointer load follows, the order of C statements controls whether `andi` (mask) or `lw` (pointer load) comes first:

```c
// Generates: andi → lw → sll (mask first, pointer load fills stall)
s32 idx = var & 0xFFFF;
s32 fgAddr = (s32)trackGeom->faceGroups;
temp = ((idx << 3) + idx) << 2;

// Generates: lw → andi → sll (pointer load first — WRONG for some targets)
temp = (((var & 0xFFFF) << 3) + (var & 0xFFFF)) << 2;
// fgAddr loaded separately
```

Splitting the mask into a separate statement (`s32 idx = var & 0xFFFF`) forces the compiler to evaluate it first. The subsequent pointer load (`trackGeom->faceGroups`) is then scheduled into the stall slot between the `andi` and the dependent `sll`.

## Block-Scoped Variables for Deferred Update Pattern

When a value should only be updated conditionally (e.g., updating a loop variable only when a neighbor index is non-negative), use block-scoped temporary variables to prevent the compiler from assigning a callee-saved register:

```c
// CORRECT: block-scoped neighbor uses temporary register (v1)
if (cross2d(...) > 0) {
    s16 neighbor = faceGroup->neighbor0;
    if (neighbor >= 0) {
        var_v1 = neighbor;
        goto next;
    }
}

// WRONG: function-scoped neighbor gets callee-saved register (s3)
s16 neighbor;
// ... later:
neighbor = faceGroup->neighbor0;  // generates: move s3,v0 instead of move v1,v0
```

This matters because the target code uses `move v1,v0` (temporary) not `move s3,v0` (callee-saved). The block scope limits the variable's lifetime so the compiler doesn't need to preserve it across function calls.

## Recognising Signed Division by `0x2000` (and Other Powers of Two)

The shift-and-bias pattern for signed division by `2^N` shows up in m2c output as an explicit `if (x < 0) x += (2^N - 1);` followed by `x >> N`. This is **not** a special operation — it's just `x / (2^N)` written out longhand:

```c
// Decompilation artefact — DO NOT keep this form
if (temp < 0) {
    temp += 0x1FFF;
}
result = temp >> 13;

// What the original source actually was
result = temp / 0x2000;
```

When you see this pattern, collapse it back to a plain division and let the compiler re-emit the bias. Keeping the longhand form forces extra named locals into the function, which constrains instruction scheduling (see "Inlining vs. Precomputing" considerations) and usually makes the match worse, not better.

The same applies to other powers of two: `+ 0x1` then `>> 1` is `/ 2`, `+ 0x3` then `>> 2` is `/ 4`, etc. The bias constant is always `2^N - 1`.

## Comma Operator + Ternary for Goto-Free Lazy Evaluation

When the original code uses gotos to skip an expensive computation, refactoring with a flag variable adds visible branches and instructions for the flag itself, hurting match by 2–3%. A cleaner alternative: collapse the lazy-evaluated branch into a single boolean expression using comma operators and ternary:

```c
// Original (with gotos): only computes endpointDistSq when needed
if (cond1) goto push;
if (cond2) goto push;
if (alongDist < 0) endpointDistSq = (s64)alongDist * alongDist;
else { endDist = wallLen - alongDist; endpointDistSq = (s64)endDist * endDist; }
totalDistSq = (s64)perpDist * perpDist + endpointDistSq;
if (totalDistSq >= (s64)arg3 * arg3) goto skip;
push: ... skip: ;

// Goto-free, lazy: comma operator preserves "compute only if needed" semantics
if (cond1 || cond2 ||
    (((alongDist < 0) ? (endpointDistSq = (s64)alongDist * alongDist)
                       : ((endDist = wallLen - alongDist),
                          (endpointDistSq = (s64)endDist * endDist))),
     ((totalDistSq = (s64)perpDist * perpDist + endpointDistSq) <
      (s64)arg3 * arg3))) {
    push();
}
```

In func_80060CDC_618DC, this scored 90.279% vs 89.675% for the always-compute alternative and 88.644% for the flag-variable approach. The `||` short-circuit is what preserves the "only compute when needed" behavior — the comma block is only evaluated if cond1 and cond2 are both false. Wrap the comma chain in a macro to keep the call site readable.

## Macros vs Inline Functions for Repeated Logic

Preprocessor macros (`#define ... \`) expand to identical text and produce **identical codegen** to manually inlined code. Use them freely to deduplicate repeated logic without affecting match score — this is purely a readability win. In contrast, `static inline` helpers may or may not be inlined by GCC 2.7.2 and can introduce function-call overhead or shifted register allocation when not inlined.

For func_80060CDC_618DC, factoring four near-identical wall-projection blocks into `WALL_PROJ_BASIC`/`WALL_PROJ_FULL` macros and the push action into `DO_PUSH` produced byte-identical assembly to the unfactored version (89.675% in both cases).

## Narrow Inline ASM for Isolated `mflo` Register Selection

For func_800B1544_A13F4, the natural C expression for the steering sine/cosine scratch values emitted the correct instructions but stored the multiply results via `$2` instead of the target `$12`/`$13`. Local register variables fixed that block, but GCC 2.7.2 treated `$12`/`$13` as reserved for the whole function and shifted earlier multiply temporaries to `$14`/`$15`.

A narrow inline asm block around only the multiply-result stores matched without disturbing the rest of the function:

```c
approximateSin(player->steeringAngle);
__asm__ volatile("mult $2,%0\n\tmflo $12\n\tsw $12,0x10($sp)" : : "r"(sinMul));
approximateCos(player->steeringAngle);
__asm__ volatile("mult $2,%0\n\tmflo $13\n\tsw $13,0x18($sp)" : : "r"(sinMul));
```

Use this only as a last-resort register-selection tool after confirming the surrounding C already matches. Prefer a real stack object, such as `Vec3i sp10` plus adjusted padding, so hardcoded stack offsets still correspond to meaningful local storage.

## Short-Lived Register Locals for Division Temporaries

For processSpatialAudio, the natural positional-audio attenuation logic reached 99.868% with only register-allocation differences around `(flags * distance) / (outerDistance - innerDistance)`. Broad function-scope register hints made allocation worse, but block-scoped register locals around only the attenuation expression matched exactly:

```c
#ifdef CC_CHECK
#define AUDIO_REG(reg)
#else
#define AUDIO_REG(reg) __asm__(reg)
#endif

register s32 outerDistance AUDIO_REG("$7");
register s32 flags AUDIO_REG("$3");
register s32 denominator AUDIO_REG("$2");

outerDistance = gSoundManager->audioOuterDistance;
flags = gSoundManager->renderQueue[i].flags;
denominator = outerDistance - innerDistance;
volume = (flags * distance) / denominator;
```

Use the `CC_CHECK` guard when fixed MIPS register names are needed in production source; the host-side Clang syntax check does not accept numeric MIPS register names. Keep these locals as narrow as possible, because extending their lifetime can force saved-register spills and shift unrelated loops.

## Fixed Register Locals and Host Syntax Checks

For func_800136E0_142E0, short-lived fixed register locals were needed to keep a branch-delay-slot assignment and a nearby `-1` constant in the target registers:

```c
#ifdef CC_CHECK
s32 base;
s32 temp;
s32 negOne;
#else
register s32 base __asm__("a0");
register s32 temp __asm__("v0");
register s32 negOne __asm__("a2");
#endif
```

KMC GCC accepts ABI register names without `$` for local register variables. The host-side Clang syntax pass rejects these names, so keep the fixed-register declarations behind `#ifndef CC_CHECK` and provide plain locals for syntax checking. If the target needs a constant materialized at a specific source point, an empty tied asm can pin the assignment schedule without emitting instructions:

```c
negOne = -1;
__asm__ volatile("" : "=r"(negOne) : "0"(negOne));
```

## Call Argument Register Locals for Delay Slot Scheduling

For func_800B3FFC_1E10AC, the natural command-label placement code reached 99.504% but scheduled `addiu s0,s0,6` into the `getCommandEntryMasked` delay slot. The target needs that add before the call and `addiu s0,s0,4` in the delay slot.

Binding the short-lived command arguments to `a0`/`a1` and tying them through an empty asm together with the computed column value forced the desired schedule:

```c
#ifdef CC_CHECK
s32 commandCategory;
s32 commandType;
#else
register s32 commandCategory __asm__("a0");
register s32 commandType __asm__("a1");
#endif

commandCategory = entry->commandCategory;
commandType = entry->commandType;
spriteIndex = (frameDelta * 6) + 6;
__asm__ volatile(
    "" : "=r"(spriteIndex), "=r"(commandCategory), "=r"(commandType)
    : "0"(spriteIndex), "1"(commandCategory), "2"(commandType)
);
cmd = getCommandEntryMasked(commandCategory, commandType);
spriteIndex += 4;
```

Declaring the local call prototype with `s32` arguments avoided extra `andi` instructions that appeared when `u8` temporaries were passed through the fixed registers. Keep these register locals narrow and use named ABI registers with a `CC_CHECK` fallback.

## Validate Permuter Candidates with the Local Harness

For func_80060CDC_618DC_618DC, decomp-permuter candidates scored differently under the permuter's imported source than under the local `./build.sh` harness. Some candidates that looked better internally regressed badly when compiled through the function directory's actual setup. Always rerun promising permuter output through `./build.sh <candidate>.c` before adopting changes.

## Dead Temporaries Can Improve KMC Scheduling

For func_80060CDC_618DC_618DC, using dead scalar temporaries for equivalent constants or pointer bases changed instruction scheduling without changing semantics. Two useful examples were routing the full-edge push denominator through a dead vertex temporary and caching one face-group pointer in a `TrackFaceGroup *` local before an edge check. These tricks are highly local: the same pointer-temp pattern improved edge 3 but regressed edge 2 and edge 0.

## Void Public APIs Can Still Need Direct Casts

For func_80060CDC_618DC_618DC, the public prototype needs to stay `void *` because callers pass several layout-compatible track data pointers that are not typed as `TrackGeometryFaceData *`. Adding typed local aliases inside the matched function shifted the stack slots for saved arguments and regressed the match. Direct casts at each dereference preserved the original stack layout while keeping field accesses typed.

The final endpoint distance check also needed a no-code clobber barrier after `perpDistSq = ((s64)dx) * dx`:

```c
#ifndef CC_CHECK
__asm__ volatile("" : : : "t0", "t1", "t2", "t3", "t4", "t5", "t8", "t9", "s0", "s1");
#endif
```

This forced KMC GCC to keep the 64-bit product in `t6/t7` and emit the spill pattern expected by the target. A real inline store duplicated the spill; the empty clobber was enough.

## Prefer Explicit Overlay Layouts in race_hud.c

While consolidating `race_hud.c` callback state structs, anonymous union/struct overlays were not a useful way to expose alternate field names for the same allocation: the syntax-check path did not expose members reliably, and the struct-offset formatter treated anonymous overlay fields like sequential storage. Use one explicit canonical layout instead, then access existing fields by their stable names or cast only at API boundaries that require a different view.

## KMC Assembler Can Be Symbol-Name Sensitive

Renaming functions in `geometry.c` can change the assembled bytes even when KMC GCC emits identical `.s` apart from the label text. For example, renaming `func_8006BDBC_6C9BC` to `multiplyMatrixRotation` made the assembler insert an extra `nop` between `rotateVectorY` and the renamed function, shifting the next symbol by four bytes. Similarly, `func_mulMatrix3x3T` assembled correctly as `matrixMultiplyTransposed`, but `multiplyMatrixByTranspose` shifted the helper by four bytes.

When renaming tightly packed geometry helpers, test candidate names by compiling a temporary object and comparing both symbol offsets and disassembly, then run `./tools/build-and-verify.sh`. The `func_` prefix itself is not always required for a match; the accepted `matrixMultiplyTransposed` name keeps the original checksum.

## Preserve Volatile Display List Pointer Declarations

`gDisplayListAllocPtr` is intentionally declared as `Gfx *volatile` in `model_loader.c`. Moving it to a shared non-volatile declaration in `graphics.h` let KMC GCC optimize away intermediate stores in `buildAuxBufferDisplayList`, shortening the function by 0x54 bytes and shifting the whole main data segment by 0x50. Keep volatile-sensitive globals declared with the qualifier required by each translation unit, even if that means retaining a local extern.

## Distinguish Snowboard Stats from Board Models

The player fields at `0xBBA` and `0xBBB` describe different snowboard concepts. `boardModelId` at `0xBBA` selects the character-specific board model variant, while `snowboardId` at `0xBBB` selects one of the 18 performance/effect profiles. The latter was previously called `costumeID`, which obscured special-board checks and stat-table indexing.

Secondary item 5 is the rocket boost and item 10 is wings. The Dragon snowboard continuously activates both effects. Wings use the player fields at `0xBA6` and `0xBD1` for their timer and active flag and reduce gravity while active; they are unrelated to the game's ghost attacks, Haunted House ghosts, and replay ghost data.

## Reuse Canonical Types Across Allocation and Task Callbacks

Partial `GameState` overlays that expose the player-array pointer at offset `0x10` should use `GameState` and `Player`, rather than declaring truncated duplicates for fields needed at high offsets. The former `PlayerInfo.animationFlags` field at `0xB84` exactly overlapped `Player.animationFlags`, and the former `InitPlayerData.spriteGroupIndex` at `0xBB9` was really `Player.characterId`. Reusing the canonical types prevents these parallel layouts and names from drifting apart.

Scheduled task callbacks also receive different phases of the same allocation. Init, update, cleanup, and spawn helpers should share the complete task state when their fields occupy the same offsets; do not create a new padded view for each callback. When an element is exactly a `SpriteRenderArg`, use that existing type rather than duplicating its layout with alternate field names.

A task prefix containing a `Transform3D` at `0x00`, a display-list pointer at `0x20`, segment pointers at `0x24` through `0x2C`, and render state through `0x3B` is a complete `DisplayListObject`. Linda's Castle's flying-enemy init and update callbacks formerly described this same prefix as unrelated padded fields; embedding `DisplayListObject` unified the callback layouts and preserved the matching code.

KMC GCC can still be sensitive to the source shape of typed player indexing. In `initRaceProgressIndicatorTask`, replacing the byte-offset induction variable with `&gameState->players[i]` emitted the operands of one `addu` in the opposite order and broke the ROM checksum. Keeping the offset derived from `sizeof(Player)` preserves the canonical element size while matching the target instruction encoding.

High-offset fields in a partial `GameState` overlay can also be later elements of the canonical player array. The offsets formerly named `unk101C` and `unk1C04` are `players[1].worldPos` and `players[2].worldPos`: each advances by exactly `sizeof(Player)` from `players[0].worldPos`. Check such offsets against the canonical array stride before extending a local overlay.

Preserve local allocation size when introducing a smaller canonical type. `updateScriptedCamera` only treats its temporary camera data as a `Transform3D`, but its target stack frame reserves `0x30` bytes rather than `sizeof(Transform3D)`. A union containing the typed transform and a byte array of the original size exposes the canonical fields without changing the stack frame.

Typed array indexing may also change KMC's loop strength reduction. The audio listener-copy loops no longer matched when their destination was written directly as `&soundManager->listenerTransforms[i]`. Deriving the original address expression from `sizeof(Transform3D)` and the `listenerTransforms` member offset retains type-backed layout information while preserving the target operand and instruction order.

The Ice Land boss model initializer has a related overlapping-layout constraint. `Player` stores asset pointers in
the first `0x38` bytes, followed by `DisplayListObject` elements of size `0x3C`. KMC only reproduced the target loop
when a `0x3C` stride view advanced from the `Player` base and a second typed view exposed the display object at
offset `0x38`; iterating `Player.bodyPartDisplayObjects` directly added two instructions. A small documented adapter
view can preserve this induction shape while the function signatures and all actual field accesses continue using
the canonical `Player` and `DisplayListObject` types. The same two views match the Crazy Jungle and Jingle Town boss
initializers, so keep them shared in `gamestate.h` instead of recreating boss-local padding structs or using byte
offset casts.

## Preserve Historical ABIs When Centralizing Prototypes

Moving local extern declarations into shared headers can change KMC GCC output even when a caller ignores the
return value. `onTrickCompletedHook` has an empty body and behaves like a `void` hook, but its original `s16`
prototype affects instruction scheduling in `beginPostTrickSlidingStep`. Changing the declaration to `void`
reordered three stores and broke 12 ROM bytes. Keep the historical non-void ABI and narrowly suppress the
checker warning on the empty implementation.

Argument signedness is similarly codegen-sensitive at call sites. For example, the final render-flags argument
to `setupAndEnqueueSprite` and the cutscene frame index must retain their signed types so callers emit the target
sign-extension sequence. When factoring headers, derive prototypes from both the definition and caller
assembly rather than widening small integer arguments to a convenient unsigned type.

## Preserve Combined Halfword Views When Consolidating Byte Fields

The credits task allocation uses several adjacent byte fields through both byte and halfword operations. The
corner `TextRenderArg` color bytes at `0x0A` are initialized and faded with `sh`, while the renderer reads the
alpha byte at `0x0B`. Likewise, subtitle style and alpha pairs at `0x978`, `0x97A`, `0x98C`, and `0x98E` are
initialized with halfword stores but later consumed as individual bytes. Model these pairs with named unions
that expose both the `s16` value and its high/low bytes; splitting them into standalone `u8` fields changes KMC
stores and register allocation.

Do not merge adjacent pointers merely because two partial allocation views gave them similar names. In
`CreditsState`, `0x964` is the subtitle text table and `0x968` is the scrolling-credits text table. Keeping both
typed and separately named made it possible to replace three padded state copies with one canonical layout
without losing the distinction between their consumers.

The remaining credits render records are also canonical shared types: the records at `0x96C` and `0x980` are
`TextLayoutArg`, while the arrays at `0x9D0` and `0xB80` contain `FrameSpriteEntry` elements. Keep the combined
halfword views needed by credits on those shared graphics types rather than maintaining credits-only copies;
this preserves the original `sh` instructions while giving every consumer the renderer's semantic field names.

The credits palette ROM table uses the shared `CompressedAssetMeta` layout. The credits character task's
halfword at `0x08` is a packed asset-pair selector: credits writes it with `sh`, while model initialization reads
the low byte as the asset-pair index. A named union preserves both access widths without casts or manual offsets.

The offset-based table consumed by `credits_subtitles.c` is specifically a subtitle schedule. Each eight-byte
entry contains a subtitle start frame, a command count, and a relative offset to its command bytes. Typing and
naming that format around its sole consumer makes the schedule semantics explicit without introducing a second
partial view of the credits task allocation.

The training instruction panel has the same mixed-width pattern at offsets `0x14` and `0x16`: each color
component is animated with signed halfword operations, while rendering reads its low byte on the big-endian
target. A signed-halfword/byte union preserves both access widths and reveals that the old `scale` byte was
actually the high byte of the panel's green component. This also allowed the initialization and runtime
partial structs to collapse into one canonical task state without changing code generation.

## Check Adjacent BSS Symbols for One Aggregate

The audio command-list array at `0x800A8D90` and the former partial audio-manager object at `0x800A8D98` are one
`AudioManager` workspace. The two command-list pointers are the first fields, followed by the audio-info ring,
thread, retrace queue, and task-completion queue. Treating the latter fields as a separate object obscured why
the completion queue was addressed as element `0x80` of the command-list symbol.

When a field expression appears to index far beyond a small BSS array, inspect the immediately adjacent symbols
before preserving the apparent alias. A shared aggregate can reveal the real field and remove manual-looking
addressing. Preserve the aggregate's original start alignment and total byte size; here, one aligned `0x238`-byte
definition generated the same BSS layout and restored the original KMC instruction that derives the completion
queue from the already-loaded manager base.

The audio task embedded at offset `0x08` of each `AudioInfo` is the SDK's canonical `OSTask`, not a game-specific
copy of its sixteen fields. Embedding `OSTask` retains the exact offsets and code generation while giving its
microcode, data, stack, output, and yield fields their standard names.

## Preserve Raw Table Indexing When Struct Indexing Changes Address Formation

Giving a packed table a shared entry type does not guarantee that indexing it through that type will preserve
KMC GCC's address calculation. In `checkClocktowerLocationDiscovery`, changing the original `s16` indexing of
`storyMapAngleBounds` to `TriggerAngleBoundsTable[locationId].minAngle` and `.maxAngle` shortened the function
and changed register allocation, despite producing the same offsets. Keep the shared entry definition for
layout documentation, but retain the original scalar indexing expression in matching functions when the typed
form changes code generation.

## Trace Partial Task Structs Through Their Callers

A small padded struct used by one function may be a partial view of the caller's task state rather than a
distinct object. `TownController` exposed only a word at offset `0x4C`, but its sole caller cast a
`StoryMapCameraState *` to that type. The field is the camera's `orbitRadius`, which the collision resolver
recomputes after moving the camera away from an NPC.

When a call requires a cast between unrelated task structs, compare the accessed offsets with the caller's
complete layout before preserving the partial type. Moving the complete type to its subsystem header can remove
the duplicate, the padding, and the cast without affecting code generation.

`scheduleTask()` returns the address of the new node's inline payload, which is also the pointer passed to the
task's callbacks. When a scheduler only initializes a trailing callback-state field, type the return value as
the callback's complete state instead of recreating `Node` plus padding to reach that field. Crazy Jungle's rock
spawner can therefore set `FallingRockHazard.positionIndex` directly without a scheduler-node overlay.

## Recognize DisplayListObject Prefixes in Render Tasks

Level render-task structs that begin with a `Transform3D`, followed by display-list and segment pointers at
offsets `0x20` through `0x2C`, contain the canonical `DisplayListObject` as their prefix. The Snowboard Street
rotating-sky tasks were split into separate initialization and rendering views: one exposed translation and
asset fields, while the other exposed the transform and a trailing rotation angle. Embedding
`DisplayListObject` in one shared task type removes both partial copies and gives the fields their renderer
semantics without casts or manual offsets.

An embedded prefix at offset zero preserves KMC GCC 2.7.2 code generation when passing its address to renderer
functions or accessing nested transform translations. Also inspect asset-table layouts before retaining byte
offset arithmetic: the rotating sky's `base + 0x90` display-list pointer is the named
`LevelDisplayLists.sceneryDisplayLists1` field.

## Compare Allocation Overlays With the Shared Game State

Task code that calls `getCurrentAllocation()` may define a padded local allocation struct even when the active
allocation is the race `GameState`. Compare every exposed offset with `GameState` before retaining the overlay.
The Shoot Cross target pointer at offset `0x24`, player array at `0x10`, and hit counter at `0x5A` all belong to
one `GameState`; adding the missing target pointer to the shared definition removed two partial copies and
revealed that the apparent projectile hit counter was the race-wide Shoot Cross score.

Jingle Town's train allocation view likewise mapped directly to `GameState.gameData` at `0x30`,
`memoryPoolId` at `0x5C`, and `gamePaused` at `0x76`. Using the complete game state preserved code generation
and showed that the train's former `unk56` is its current track sector. Its cleanup view was also just the
`DisplayListObject` prefix of the train task: offsets `0x24` and `0x28` are the standard `segment1` and
`segment2` asset pointers. Check both the active allocation and embedded renderer prefixes before retaining a
padded one-function struct.

Haunted House's allocation overlay demonstrated that matching offsets matters more than a local field's guessed
name: the byte it called `memoryPoolId` was at `0x5E`, which is actually `GameState.numPlayers`, not the real
pool ID at `0x5C`. Its sprite tasks also contain `loadAssetMetadata_arg` beginning at offset `0x04`; embedding
that shared sprite state gives the position, texture pointers, and alpha their canonical layout while preserving
code generation. When a render callback receives the original task payload, extend that task's shared type with
the callback-owned trailing fields instead of defining a second render-state view of the same bytes.

Sunny Mountain shows that the same renderer prefix can appear after task-specific data. Its chair-lift task
stores two `Vec3i` endpoints before a complete `DisplayListObject` at offset `0x18`, while the flying-bird task
starts with the object at offset zero. In both cases, embedding the shared type exposes the transform and segment
pointers without changing their offsets. The race-data pointer used for chair positions is also
`GameState.raceTransformData` at offset `0x48`, not a level-specific allocation field.

Character select demonstrates that a mode-specific allocation should have one canonical state type rather than
partial `GameState` views in each renderer. KMC is sensitive to the operand order used to address its
`Transform3D` array: ordinary typed array indexing reverses the otherwise equivalent `addu` operands. A shared
accessor whose offset comes from the canonical field, with the element stride written before the allocation base,
keeps the layout type-derived while preserving the original instruction order.

The save-slot screen demonstrated both patterns at once. Its graphics file had four padded views of the same
`SaveSlotScreenState`, while its controller recreated trailing fields from four scheduled graphics-task states.
Sharing the complete screen and task definitions exposed the EEPROM slot records, menu state, selected slot, and
animation fields directly. Its sprite and text entries are also the canonical `TextRenderArg`, `SpriteRenderArg`,
`ScaledSpriteArg`, `ColoredTextRenderArg`, and `TextLayoutArg` types. When KMC originally stores a packed
padding-and-shade pair with `sh`, keep the shared byte-oriented renderer type but write the pair through its
halfword address; assigning only the named shade byte changes code generation and can lengthen the function.

## Consolidate Root Game-Mode Task State Across Dispatchers

The story, versus, and main mode dispatchers each allocate the same four-byte task state: a signed halfword
state at offset `0x0`, a byte substate at offset `0x2`, and alignment padding at offset `0x3`. These are one
shared `GameModeTaskState`, not address-specific local types. Defining the complete layout in the session
manager header and allocating it with `sizeof(GameModeTaskState)` preserves the original four-byte allocation
and store ordering in KMC GCC 2.7.2.

When identical small scheduler allocations appear in multiple mode initializers, compare their field widths,
offsets, allocation sizes, and initialization order before keeping local copies. If no narrower behavior is
observed, conservative `state` and `substate` names communicate their role without inventing unsupported
semantics.

## Distinguish Mode-Specific Allocation Overlays From GameState

`getCurrentAllocation()` is not always a race or story `GameState`. The title screen allocates two complete
`ViewportNode` values at offsets `0x000` and `0x1D8`, where `GameState` instead exposes race pointers and audio
state. Title render tasks that cast this allocation to `GameState` only happened to reach the right offsets.
Give a mode-specific allocation one shared subsystem type and use it in every task that consumes that mode,
rather than retaining a file-local copy or borrowing unrelated `GameState` field names.

The title tasks also showed that partial UI structs often contain canonical renderer state. Their menu options
are `TextRenderArg` values, their legal notices are adjacent `SpriteRenderArg` values, and the
title logo starts with `TileMapScrollRenderState`. Embedding those common types preserves their offsets and KMC
code generation while replacing duplicated `asset`, sprite-index, alpha, and tile fields with renderer-owned
names. Conversely, bytes `0x3B0` through `0x3CF` in the story `GameState` form one complete `Transform3D`;
representing that range as the transform removes title-only aliases from the common game-state layout.

When naming sprite fields, inspect the decompressed sprite frames rather than inferring their purpose solely
from screen position or neighboring menu state. In the title sprite sheet, frames 5 and 6 are the copyright and
Nintendo license notices; treating them as player-count prompts produced plausible layouts but incorrect
semantic names.

## Resolve Overlay Function Aliases by Payload Layout and Behavior

Player count select scheduled `setPlayerBehaviorMode` through an address shared with the race overlay, then
wrote byte `0x2B` of the returned task payload. In the player-select overlay that address is actually
`initPlayerSelectSprites`, and `0x2B` is the named `PlayerSelectState.slotIndex` field. When an overlay callback
appears to consume an impossible common type such as a full race `Player`, compare the active overlay, callback
behavior, task allocation size, and payload offsets before preserving the borrowed symbol.

Player-select task payloads also duplicate complete renderer arguments. Its portrait entries are
`ScaledSpriteArg`, its four player-count options are `TextRenderArg`, and its header and indicator tasks are
`SpriteRenderArg`. Packed halfword stores spanning renderer padding and shade bytes can be represented by a
named union in the common renderer type; this removes manual byte addressing while retaining KMC's original
`sh` code generation for every consumer.

## Replace Offset-Only Cutscene Manager Views With the Canonical Type

Cutscene callbacks frequently receive the complete `CutsceneManager` even when a local struct exposes only one
field near the end of it. The cutscene wait path had separate padding-based views for `enableTransparency` at
`0xFF5`, `skipAnimation` at `0xFF7`, and `sceneRenderNode` at `0xFF8`. Using `CutsceneManager` directly removes
those copies and preserves KMC GCC 2.7.2 code generation, including when the manager is stored as the first
field of a scheduled task or passed indirectly through a callback payload.

Scheduled-task producers should also use the complete task state declared by the task's owning subsystem. The
cutscene system's two-field wait-task view was the prefix of `CutsceneWaitMenuState`, while the menu's local
16-bit byte union was already represented by `TextLayoutColorValue`. Consolidating both types exposes the
manager, table, text-render resources, panel dimensions, and color channels without padded local overlays.

## Consolidate UI Allocation Views Around Renderer-Owned Types

The gallery used separate menu, viewer-render, and allocation structs for the same `0xCC0` mode allocation,
plus two identical viewer-task prefixes. One `GalleryMenuState` and one `GalleryViewerState` expose which
fields belong to the persistent screen and which belong to the scheduled viewer task. This also prevents a
prefix-compatible cast from disguising the gallery's selected category as the viewer task's navigation state.

Most of the gallery allocation is shared renderer state: `TileMapScrollRenderState`, `FrameSpriteEntry`,
`TextRenderArg`, `SpriteRenderArg`, `ColoredTextRenderArg`, `TextData`, and `Transform3D`. Match local fields by
exact offset before assigning semantics. In particular, the gallery's byte at offset `0x0D` of each item sprite
is `TextRenderArg.overridePaletteCount`, not transparency (which is at `0x0E`). A plausible local field name can
therefore be wrong even when its surrounding struct size matches. Packed padding-and-shade halfwords should be
represented by a union in the shared renderer type so KMC can retain the original `sh` store.

## Preserve Packed Global Fields When Naming Their Byte Semantics

`AssetGroup` stores an animation-asset index and shadow scale as the high two bytes of one packed `s32`.
Replacing that word with separate `s8`, `u8`, and `u16` fields preserved the structure offsets but changed KMC's
alignment of the following globals, growing the ROM data segment. Keep the original word in the containing
structure and define a shared union view with named byte fields for code that needs those semantics. This removes
file-local padded views without changing the declared storage that controls global-data placement.

## Use Renderer Types in Standalone Splash Allocations

The logo splash's `0x2F0` scheduler allocation is a standalone UI state, not a partial `GameState`. Its two
`0x2C` padded tile-map views are complete `TileMapScrollRenderState` values, and each `0x18` footprint entry is
a complete `FrameSpriteEntry`. Embedding those canonical types and typing the loaded assets as
`TileMapTextureAsset` and `SpriteSheetData` removes local copies while preserving the allocation size and all
field offsets.

When replacing a local sprite copy whose shade was declared as a single `s16`, assign
`FrameSpriteEntry.shade.shadeWithPadding` rather than only its named intensity byte. This retains KMC's original
halfword store while still using the common renderer definition.
