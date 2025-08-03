#include "56910.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_216290);
USE_ASSET(_34F7E0);
USE_ASSET(_34CB50);
USE_ASSET(_34F9A0);
USE_ASSET(_350140);
USE_ASSET(_3505F0);

extern s32 D_B7E70;
extern s32 D_B9C40;
extern s32 D_80091550_92150[];
extern s32 D_80091510_92110[];
extern Asset D_80091250_91E50[];
extern Asset D_80091310_91F10[];
extern Asset D_80091450_92050[];
extern AssetWithoutSize D_800913D0_91FD0[];
extern func_80055E68_56A68_result *D_80091590_92190[];
extern D_80090F90_91B90_item D_80090F90_91B90[];

D_80090F90_91B90_item *func_80055D10_56910(s32 index) {
    return &D_80090F90_91B90[index];
}

void *func_80055D34_56934(s32 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80091250_91E50[index].start,
        D_80091250_91E50[index].end,
        D_80091250_91E50[index].size
    );
}

void *func_80055D7C_5697C(s32 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80091310_91F10[index].start,
        D_80091310_91F10[index].end,
        D_80091310_91F10[index].size
    );
}

void *func_80055DC4_569C4(s32 index) {
    return dmaRequestAndUpdateState(D_800913D0_91FD0[index].start, D_800913D0_91FD0[index].end);
}

void *func_80055DF8_569F8(s32 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80091450_92050[index].start,
        D_80091450_92050[index].end,
        D_80091450_92050[index].size
    );
}

s32 func_80055E40_56A40(s32 arg0) {
    return D_80091510_92110[arg0];
}

s32 func_80055E54_56A54(s32 arg0) {
    return D_80091550_92150[arg0];
}

func_80055E68_56A68_result *func_80055E68_56A68(s32 index) {
    return D_80091590_92190[index];
}

void *loadAsset_B7E70(void) {
    return dmaRequestAndUpdateState(&D_B7E70, &D_B9C40);
}

void *loadAsset_216290(void) {
    return dmaRequestAndUpdateStateWithSize(&_216290_ROM_START, &_216290_ROM_END, 0x5740);
}

void *loadAsset_34CB50(void) {
    return dmaRequestAndUpdateStateWithSize(&_34CB50_ROM_START, &_34CB50_ROM_END, 0x5E28);
}

void *loadAsset_34F9A0(void) {
    return dmaRequestAndUpdateStateWithSize(&_34F9A0_ROM_START, &_34F9A0_ROM_END, 0xE08);
}

void *loadAsset_350140(void) {
    return dmaRequestAndUpdateStateWithSize(&_350140_ROM_START, &_350140_ROM_END, 0x868);
}

void *loadAsset_3505F0(void) {
    return dmaRequestAndUpdateStateWithSize(&_3505F0_ROM_START, &_3505F0_ROM_END, 0xA88);
}

void *loadAsset_34F7E0(void) {
    return dmaRequestAndUpdateStateWithSize(&_34F7E0_ROM_START, &_34F7E0_ROM_END, 0x438);
}