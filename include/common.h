#pragma once

#include "include_asm.h"
#include "ultra64.h"

void *memcpy(void *, const void *, size_t);

#define ABS(d) ((d) > 0) ? (d) : -(d)
