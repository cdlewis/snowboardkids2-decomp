#include "69EF0.h"
#include "common.h"

typedef struct {
    void *start;
    void *end;
} AssetPair;

typedef struct {
    void *start;
    void *end;
    s32 size;
} AssetWithSize;

extern AssetWithSize *D_80094600_95200[];
extern AssetPair *D_80094390_94F90[];
extern AssetWithSize D_800947E0_953E0[];
extern s32 *D_80094780_95380[];
extern s32 *D_800947B0_953B0[];
extern void *D_3ECE40;
extern void *D_3F2980;
extern AssetWithSize D_80094870_95470[];
extern AssetPair D_80094900_95500[];
extern AssetWithSize D_80094990_95590[];
extern AssetWithSize D_80094A68_95668[];

void *func_8005D990_5E590(s16 groupIndex, s16 pairIndex) {
    AssetPair *assetArray = D_80094390_94F90[groupIndex];
    return dmaRequestAndUpdateState(assetArray[pairIndex].start, assetArray[pairIndex].end);
}

void *func_8005D9D0_5E5D0(s16 groupIndex, s16 pairIndex) {
    AssetWithSize *assetArray = D_80094600_95200[groupIndex];
    return dmaRequestAndUpdateStateWithSize(assetArray[pairIndex].start, assetArray[pairIndex].end, assetArray[pairIndex].size);
}

void *func_8005DA20_5E620(s16 index) {
    if (index == 0xA) {
        return NULL;
    } else {
        return dmaRequestAndUpdateStateWithSize(D_800947E0_953E0[index].start, D_800947E0_953E0[index].end, D_800947E0_953E0[index].size);
    }
}

s32 func_8005DA84_5E684(s16 groupIndex, s16 pairIndex) {
    s32 *group = D_80094780_95380[groupIndex];
    return group[pairIndex];
}

s32 func_8005DAAC_5E6AC(s16 groupIndex, s16 pairIndex) {
    s32 *group = D_800947B0_953B0[groupIndex];
    return group[pairIndex];
}

void *func_8005DAD4_5E6D4(void) {
    return dmaRequestAndUpdateStateWithSize(&D_3ECE40, &D_3F2980, 0xA518);
}

void *func_8005DB00_5E700(s32 index) {
    return dmaRequestAndUpdateStateWithSize(D_80094870_95470[index].start, D_80094870_95470[index].end, D_80094870_95470[index].size);
}

void *func_8005DB48_5E748(s16 index) {
    return dmaRequestAndUpdateState(D_80094900_95500[index].start, D_80094900_95500[index].end);
}

void *func_8005DB80_5E780(s16 index) {
    return dmaRequestAndUpdateStateWithSize(D_80094990_95590[index].start, D_80094990_95590[index].end, D_80094990_95590[index].size);
}

void *func_8005DBD0_5E7D0(s16 index) {
    s16 clamped_index;

    if (index == 0x10) {
        return 0;
    }

    clamped_index = index;
    if (index >= 0x10) {
        clamped_index = 0;
    }

    return dmaRequestAndUpdateStateWithSize(D_80094A68_95668[clamped_index].start, D_80094A68_95668[clamped_index].end, D_80094A68_95668[clamped_index].size);
}

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DC48_5E848);

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DC60_5E860);

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DD20_5E920);

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DD90_5E990);

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DDD8_5E9D8);

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DE04_5EA04);

INCLUDE_ASM("asm/nonmatchings/5E590", func_8005DE30_5EA30);
