#include "common.h"

extern s16 signedAngleDifference(s16, s16);

void func_8002C570_2D170(void *arg0) {
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 var_a0;
    s16 var_v0;
    s16 var_v1;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s2;
    void *temp_v0;
    void *var_s0;
    void *var_s0_2;

    temp_v0 = getCurrentAllocation();
    var_s2 = 0;
    var_s1 = 0;
    if (*(s32 *)((u8 *)temp_v0 + 0x41C) > 0) {
        var_s0 = arg0;
        do {
            temp_v1 = *(s16 *)((u8 *)var_s0 + 0x32);
            switch (temp_v1) {
                case 0:
                    temp_v0_2 = signedAngleDifference(*(u16 *)((u8 *)var_s0 + 0x30), *(u16 *)((u8 *)var_s0 + 0x2E));
                    if (!(temp_v0_2 & 0x8000)) {
                        *(u8 *)((u8 *)var_s0 + 0x36) = 0;
                    } else {
                        *(u8 *)((u8 *)var_s0 + 0x36) = 1;
                    }
                    var_v0 = temp_v0_2;
                    if (var_v0 < 0) {
                        var_v0 = -var_v0;
                    }
                    *(s16 *)((u8 *)var_s0 + 0x50) = 2;
                    *(u8 *)((u8 *)var_s0 + 0x37) = 1;
                    if (var_v0 >= 0xAAB) {
                        *(u16 *)((u8 *)var_s0 + 0x30) = (s16)((*(u16 *)((u8 *)var_s0 + 0x30) + 0x1000) & 0x1FFF);
                        *(s16 *)((u8 *)var_s0 + 0x50) = 1;
                        *(u8 *)((u8 *)var_s0 + 0x37) = 0;
                        if (!(signedAngleDifference(*(u16 *)((u8 *)var_s0 + 0x30), *(u16 *)((u8 *)var_s0 + 0x2E)) &
                              0x8000)) {
                            *(u8 *)((u8 *)var_s0 + 0x36) = 0;
                        } else {
                            *(u8 *)((u8 *)var_s0 + 0x36) = 1;
                        }
                    }
                    *(s16 *)((u8 *)var_s0 + 0x32) = 1;
                    break;
                case 1:
                    temp_v0_3 = signedAngleDifference(*(u16 *)((u8 *)var_s0 + 0x30), *(u16 *)((u8 *)var_s0 + 0x2E));
                    var_v1 = temp_v0_3;
                    if (temp_v0_3 < 0) {
                        var_v1 = -var_v1;
                    }
                    var_a0 = 0xA0;
                    if (var_v1 < 0xA0) {
                        if (*(s16 *)((u8 *)var_s0 + 0x50) == 2) {
                            *(s16 *)((u8 *)var_s0 + 0x50) = 0;
                        }
                        var_a0 = var_v1;
                        *(s16 *)((u8 *)var_s0 + 0x32) = 3;
                    }
                    if (*(u8 *)((u8 *)var_s0 + 0x36) != 0) {
                        var_a0 = -var_a0;
                    }
                    *(u16 *)((u8 *)var_s0 + 0x30) = (s16)((*(u16 *)((u8 *)var_s0 + 0x30) + var_a0));
                    if (*(s16 *)((u8 *)var_s0 + 0x50) == 1) {
                        if (*(u8 *)((u8 *)var_s0 + 0x37) != 0) {
                            *(s16 *)((u8 *)var_s0 + 0x50) = 0;
                        }
                    }
                    break;
                case 3:
                    if (*(u8 *)((u8 *)var_s0 + 0x37) != 0) {
                        *(s16 *)((u8 *)var_s0 + 0x50) = 0;
                    }
                case 2:
                    var_s2 += 1;
                    break;
            }
            var_s1 += 1;
            var_s0 += 0x64;
        } while (var_s1 < *(s32 *)((u8 *)temp_v0 + 0x41C));
    }
    var_s1_2 = 0;
    if ((var_s2 & 0xFF) == *(s32 *)((u8 *)temp_v0 + 0x41C)) {
        *(u8 *)((u8 *)temp_v0 + 0x42A) = 0;
        if (*(s32 *)((u8 *)temp_v0 + 0x41C) > 0) {
            var_s0_2 = arg0;
            do {
                *(u16 *)((u8 *)var_s0_2 + 0x2C) = *(u16 *)((u8 *)var_s0_2 + 0x2E);
                *(s16 *)((u8 *)var_s0_2 + 0x50) = *(u16 *)((u8 *)var_s0_2 + 0x56);
                setAnimationIndex(*(void **)((u8 *)var_s0_2 + 0x0), *(s8 *)((u8 *)var_s0_2 + 0x5F));
                var_s1_2 += 1;
                var_s0_2 += 0x64;
            } while (var_s1_2 < *(s32 *)((u8 *)temp_v0 + 0x41C));
        }
    }
}
