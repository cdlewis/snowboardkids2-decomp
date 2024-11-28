#ifndef HASM_H
#define HASM_H

#define LEAF(x)                 \
    .align 2                   ;\
    .globl x                   ;\
    .type x, @function         ;\
    x:                         ;\
        .ent x, 0              ;\
        .frame $sp, 0, $ra

#define END(x)                  \
    .size x, . - x             ;\
    .end x

#define SYM_START(x)            \
    .align 2                   ;\
    .globl x                   ;\
    .type	 x, @object        ;\
    x:

#define SYM_END(x)              \
    .size x, . - x

#define EXPORT(x)               \
    .globl  x                  ;\
    x:

#define ABS(x, y)               \
    .globl  x                  ;\
    x = y

.macro move dst, src
    addu \dst, \src, $zero
.endm

// divu dst
.macro divu_ds dst, dividend, divisor
    .set noreorder
    divu        $zero, \dividend, \divisor
    bnez        \divisor, 0f
     nop
    break       7
0:
    mflo        \dst
    .set reorder
.endm

#define LA(dst, address) \
    lui         dst, %hi(address); \
    addiu       dst, dst, %lo(address)

#endif