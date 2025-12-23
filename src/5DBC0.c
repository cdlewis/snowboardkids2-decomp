#include "5DBC0.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

s32 func_8005CFC0_5DBC0(void *arg0, u16 arg1, void *arg2, s32 arg3) {
    return func_80061A64_62664(arg0, arg1, arg2);
}

s32 func_8005CFDC_5DBDC(void *arg0, u16 arg1, void *arg2, s32 arg3, s32 arg4) {
    return func_80061D6C_6296C(arg0, arg1, arg2, arg4);
}

void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4) {
    func_80062918_63518(arg0, arg1, arg2, arg3, arg4);
}

s32 func_8005D020_5DC20(void *arg0, u16 arg1) {
    return func_80062274_62E74(arg0, arg1);
}

void func_8005D03C_5DC3C(Player *arg0) {
    s32 velX;
    s32 velY;
    s32 velZ;
    s64 sum;
    s32 magnitude;
    s32 maxMagnitude;

    velX = arg0->unk44C;
    velY = arg0->unk450;
    velZ = arg0->unk454;

    sum = (s64)velX * velX + (s64)velY * velY + (s64)velZ * velZ;
    magnitude = isqrt64(sum);

    maxMagnitude = arg0->unkAA4;
    if (maxMagnitude < magnitude) {
        arg0->unk44C = (s32)(((s64)arg0->unk44C * arg0->unkAA4) / magnitude);
        arg0->unk450 = (s32)(((s64)arg0->unk450 * arg0->unkAA4) / magnitude);
        arg0->unk454 = (s32)(((s64)arg0->unk454 * arg0->unkAA4) / magnitude);
    }
}

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D180_5DD80);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D308_5DF08);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D48C_5E08C);

extern u8 D_80094180_94D80[];
extern u8 D_800941BC_94DBC[];
extern s32 func_8005DDD8_5E9D8(u8, u8);

void func_8005D61C_5E21C(Player *arg0) {
    s32 temp_v1;
    s32 var_s0;
    s32 var_s0_2;
    u8 temp_s0;
    u8 *var_s2;
    u8 *var_s2_2;
    u8 *ptr;

    if (((GameState *)getCurrentAllocation())->unk7A == 0xB) {
        temp_v1 = arg0->unkB84;
        if (temp_v1 & 4) {
            arg0->unkB84 = temp_v1 | 8;
            var_s0 = 0;
            var_s2 = (u8 *)arg0;
            do {
                *(s32 *)(var_s2 + 0x58) =
                    (s32)loadAssetByIndex_95380(arg0->unkBB9, arg0->unkBBA) + D_800941BC_94DBC[var_s0] * 0x10;
                var_s0 += 1;
                var_s2 += 0x3C;
            } while (var_s0 < 0x10);
        } else {
            arg0->unkB84 = temp_v1 & ~8;
            var_s0 = 0;
            var_s2 = (u8 *)arg0;
            do {
                *(s32 *)(var_s2 + 0x58) = (s32)loadAssetByIndex_95380(arg0->unkBB9, arg0->unkBBA) + var_s0 * 0x10;
                var_s2 += 0x3C;
            } while (++var_s0 < 0x10);
        }
    } else {
        temp_v1 = arg0->unkB84;
        if (temp_v1 & 4) {
            arg0->unkB84 = temp_v1 | 8;
            var_s0_2 = 0;
            var_s2_2 = (u8 *)arg0;
            do {
                *(s32 *)(var_s2_2 + 0x58) =
                    loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + D_800941BC_94DBC[var_s0_2] * 0x10;
                var_s0_2 += 1;
                var_s2_2 += 0x3C;
            } while (var_s0_2 < 0x10);
        } else {
            arg0->unkB84 = temp_v1 & ~8;
            var_s0_2 = 0;
            var_s2_2 = (u8 *)arg0;
            do {
                *(s32 *)(var_s2_2 + 0x58) = loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + var_s0_2 * 0x10;
                var_s2_2 += 0x3C;
            } while (++var_s0_2 < 0x10);
        }
    }

    ptr = D_80094180_94D80 + arg0->unkBB9 * 5;
    temp_s0 = ptr[arg0->unkBE6];
    if (temp_s0 == 0) {
        goto copy_basic;
    }
    temp_v1 = (s32)arg0->unk20;
    if (temp_v1 == 0) {
    copy_basic:
        *(s32 *)((u8 *)arg0 + 0x23C) = (s32)arg0->unk4;
        *(s32 *)((u8 *)arg0 + 0x240) = (s32)arg0->unk8;
        return;
    }
    *(s32 *)((u8 *)arg0 + 0x23C) = temp_v1;
    *(s32 *)((u8 *)arg0 + 0x240) = (s32)arg0->unk24;
    *(s32 *)((u8 *)arg0 + 0x238) = func_8005DDD8_5E9D8(arg0->unkBB9, arg0->unkBBA) + temp_s0 * 0x10 - 0x10;
}

void func_8005D804_5E404(Player *arg0, u8 arg1, u8 arg2) {
    arg0->unkBE6 = arg1;
    arg0->unkBE7 = arg2;
}

void func_8005D810_5E410(void) {
}

extern s8 D_800941C4_94DC4[];

s16 func_8005D818_5E418(Player *arg0) {
    s16 var_a1;
    s8 temp_v0;
    s8 temp_v1;
    u16 temp_a0;
    s8 *ptr;
    s8 off;

    var_a1 = -1;
    if (arg0->unkBDA != 0) {
        temp_v1 = arg0->unkBDC & 7;
        if (temp_v1 != 0) {
            ptr = D_800941C4_94DC4;
            off = arg0->unkBB5;
            return *(ptr + ((s32)temp_v1 << 3) + off);
        }
        goto block_17;
    }
    temp_v1 = arg0->unkB7B;
    var_a1 = -(temp_v1 != 7);
    if (temp_v1 == -7) {
        var_a1 = 1;
    }
    temp_v0 = arg0->unkB7A;
    if (temp_v0 == 7) {
        var_a1 = 2;
    }
    if (temp_v0 == -7) {
        var_a1 = 3;
    }
    temp_a0 = *(u16 *)&arg0->unkB7A;
    if (temp_a0 == 0x707) {
        var_a1 = 4;
    }
    if (temp_a0 == 0xF907) {
        var_a1 = 5;
    }
    if (temp_a0 == 0x7F9) {
        var_a1 = 6;
    }
    if (temp_a0 == 0xF9F9) {
        var_a1 = 7;
    }
block_17:
    return var_a1;
}

s16 func_8005D8C8_5E4C8(Player *arg0) {
    s32 count;
    u32 temp;
    s8 temp_b7a;
    s8 temp_b7b;
    s16 temp_b7e;

    count = 1;

    if (arg0->unkBDA != 0) {
        temp = randA();
        temp = temp & 3;
        temp = temp < 1;
        count = temp + 1;
        goto end;
    }

    temp_b7a = arg0->unkB7A;
    if (temp_b7a == 7) {
        temp = arg0->unkB80;
        temp = temp ^ 7;
        temp = temp != 0;
        count = temp + 1;
    }

    if (temp_b7a == -7) {
        temp = arg0->unkB80;
        temp = temp ^ temp_b7a;
        temp = temp != 0;
        count += temp;
    }

    temp_b7b = arg0->unkB7B;
    if (temp_b7b == 7) {
        temp = arg0->unkB81;
        temp = temp ^ 7;
        temp = temp != 0;
        count += temp;
    }

    if (temp_b7b == -7) {
        temp = arg0->unkB81;
        temp = temp ^ temp_b7b;
        temp = temp != 0;
        count += temp;
    }

    temp_b7e = arg0->unkB7E;
    temp = temp_b7e & 0xC000;
    temp = temp != 0;
    count += temp;

end:
    return count;
}
