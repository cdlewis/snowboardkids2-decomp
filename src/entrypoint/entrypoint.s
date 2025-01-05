#include "hasm.h"

ABS(STACK_SIZE, BOOT_STACK_SIZE)

.section .text

.align 4

LEAF(entrypoint)
  LA($t0, main_BSS_START)
  LA($t1, main_BSS_SIZE)

.clear_bss:
  sw      $zero, 0x0($t0)
  sw      $zero, 0x4($t0)
  addi    $t0, $t0, 0x8
  addi    $t1, $t1, -0x8
  bnez    $t1, .clear_bss

.enter_program:
  LA($t2, mainproc)
  LA($sp, gMainThreadStackTop)

  jr      $t2
END(entrypoint)

.fill 0x50 - (. - entrypoint)