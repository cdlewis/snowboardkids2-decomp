#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)                   \
  __asm__(                                          \
      ".section .text\n"                            \
      "    .set noat\n"                             \
      "    .set noreorder\n"                        \
      "    .globl " #NAME ".NON_MATCHING\n"         \
      "    .type " #NAME ".NON_MATCHING, @object\n" \
      "    " #NAME ".NON_MATCHING:\n"               \
      "    .include \"" FOLDER "/" #NAME ".s\"\n"   \
      "    .set reorder\n"                          \
      "    .set at\n")
#endif

__asm__(".include \"include/macro.inc\"\n");

#endif
