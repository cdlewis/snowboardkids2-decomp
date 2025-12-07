#include "6E840.h"
#include "common.h"

typedef struct {
    u16 x;
    u16 y;
    u16 palette;
    u8 *string;
} TextData;

void enqueueTextRender(s16 x, s16 y, s16 palette, u8 *target_string, s32 arg4, s32 arg5);

void renderTextPalette(TextData *);

void func_800680F0_68CF0(Node_70B00 *arg0);