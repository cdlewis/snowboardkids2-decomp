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

## Jump Table: Empty Cases vs Identical Case Bodies

When a switch statement has two consecutive cases with identical bodies, GCC 2.7.2 merges them — both jump table entries point to the same code address. If the target ROM has one case pointing to the post-switch code instead, that case was originally empty (`case N: break;`). Check the jump table `.rodata` section to distinguish merged cases from empty ones.

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
