#include "hasm.h"

.section .text, "ax"

LEAF(rspbootTextStart)
.incbin "assets/rsp/rspboot.textbin.bin"
END(rspbootTextStart)
EXPORT(rspbootTextEnd)