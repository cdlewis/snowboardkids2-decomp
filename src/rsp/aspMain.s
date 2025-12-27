#include "hasm.h"

.section .text, "ax"

LEAF(aspMainTextStart)
.incbin "assets/rsp/aspMain.textbin.bin"
END(aspMainTextStart)
EXPORT(aspMainTextEnd)

.section .rodata, "a"

SYM_START(aspMainDataStart)
.incbin "assets/rsp/aspMain.rodatabin.bin"
SYM_END(aspMainDataStart)
EXPORT(aspMainDataEnd)
