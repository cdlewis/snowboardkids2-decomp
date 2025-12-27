#include "hasm.h"

.section .text, "ax"

LEAF(gspS2DEX_fifoTextStart)
.incbin "assets/rsp/s2dex.textbin.bin"
END(gspS2DEX_fifoTextStart)
EXPORT(gspS2DEX_fifoTextEnd)

.section .rodata, "a"

SYM_START(gspS2DEX_fifoDataStart)
.incbin "assets/rsp/s2dex.rodatabin.bin"
SYM_END(gspS2DEX_fifoDataStart)
EXPORT(gspS2DEX_fifoDataEnd)