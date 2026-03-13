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
