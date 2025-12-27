#include "hasm.h"

.section .text, "ax"

LEAF(gspF3DEX2_fifoTextStart)
.incbin "assets/rsp/f3dex2.textbin.bin"
END(gspF3DEX2_fifoTextStart)
EXPORT(gspF3DEX2_fifoTextEnd)

.section .rodata, "a"

SYM_START(gspF3DEX2_fifoDataStart)
.incbin "assets/rsp/f3dex2.rodatabin.bin"
SYM_END(gspF3DEX2_fifoDataStart)
EXPORT(gspF3DEX2_fifoDataEnd)