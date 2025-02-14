#include "common.h"
#include "initialize_video_and_threads.c"

INCLUDE_ASM("asm/nonmatchings/70DB0", thread_function_1);

INCLUDE_ASM("asm/nonmatchings/70DB0", func_800705D0_711D0);

INCLUDE_ASM("asm/nonmatchings/70DB0", func_80070650_71250);

INCLUDE_ASM("asm/nonmatchings/70DB0", thread_function_2);

#include "thread_function_3.c"

INCLUDE_ASM("asm/nonmatchings/70DB0", func_80070990_71590);

INCLUDE_ASM("asm/nonmatchings/70DB0", thread_function_4);

INCLUDE_ASM("asm/nonmatchings/70DB0", func_80070B9C_7179C);
