# Decompilation Learnings

Decompilation Learnings is a record maintained by Claude of insights from matching functions in this project. These document compiler behavior, codegen quirks, and patterns specific to KMC GCC 2.7.2 with `-O2 -mips3`.

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

outerDistance = gGraphicsManager->audioOuterDistance;
flags = gGraphicsManager->renderQueue[i].flags;
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
