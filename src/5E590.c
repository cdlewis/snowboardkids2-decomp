#include "20F0.h"
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

extern AssetPair *D_80094390_94F90[];
extern AssetPair *D_80094B28_95728[];
extern AssetPair D_80094900_95500[];
extern AssetWithSize *D_80094600_95200[];
extern AssetWithSize D_800947E0_953E0[];
extern AssetWithSize D_80094870_95470[];
extern AssetWithSize D_80094990_95590[];
extern AssetWithSize D_80094A68_95668[];
extern AssetWithSize D_80094B70_95770[];
extern AssetWithSize D_80094B70_95770[];
extern AssetWithSize D_80094C30_95830[];
extern s32 *D_80094780_95380[];
extern s32 *D_800947B0_953B0[];
extern void *D_3ECE40;
extern void *D_3F2980;
extern void *D_40A450;
extern void *D_40A590;
extern void *D_40A760;
extern void *D_40A930;

void *func_8005D990_5E590(s16 groupIndex, s16 pairIndex) {
    AssetPair *assetArray = D_80094390_94F90[groupIndex];
    return dmaRequestAndUpdateState(assetArray[pairIndex].start, assetArray[pairIndex].end);
}

void *func_8005D9D0_5E5D0(s16 groupIndex, s16 pairIndex) {
    AssetWithSize *assetArray = D_80094600_95200[groupIndex];
    return dmaRequestAndUpdateStateWithSize(assetArray[pairIndex].start, assetArray[pairIndex].end, assetArray[pairIndex].size);
}

MemoryAllocatorNode *func_8005DA20_5E620(s16 index) {
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

void *load_3ECE40(void) {
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

void *func_8005DC48_5E848(s16 index) {
    return D_80094B28_95728[index];
}

void *func_8005DC60_5E860(s32 index) {
    void *start;
    void *end;
    s32 size;
    u8 val = GameStateGet()->unk7A;

    if (val == 1) {
        return dmaRequestAndUpdateStateWithSize(&D_40A450, &D_40A590, 0x250);
    } else if (val == 2) {
        return dmaRequestAndUpdateStateWithSize(&D_40A590, &D_40A760, 0x260);
    } else if (val == 3) {
        return dmaRequestAndUpdateStateWithSize(&D_40A760, &D_40A930, 0x270);
    } else {
        return dmaRequestAndUpdateStateWithSize(D_80094B70_95770[index].start, D_80094B70_95770[index].end, D_80094B70_95770[index].size);
    }
}

s32 func_8005DD20_5E920(s32 index) {
    GameState *state = GameStateGet();
    u8 val = state->unk7A;

    if (val == 1) {
        return 0x250;
    }

    if (val == 2) {
        return 0x260;
    }

    if (val == 3) {
        return 0x270;
    }

    return D_80094B70_95770[index].size;
}

void *func_8005DD90_5E990(s32 index) {
    return dmaRequestAndUpdateStateWithSize(D_80094C30_95830[index].start, D_80094C30_95830[index].end, D_80094C30_95830[index].size);
}

MemoryAllocatorNode *func_8005DDD8_5E9D8(s16 groupIndex, s16 entityIndex) {
    return loadAssetDataByMode(groupIndex, entityIndex, MODE_DIRECT_FETCH);
}
MemoryAllocatorNode *func_8005DE04_5EA04(s16 groupIndex, s16 entityIndex) {
    return loadAssetDataByMode(groupIndex, entityIndex, MODE_DMA);
}
MemoryAllocatorNode *func_8005DE30_5EA30(s16 groupIndex, s16 entityIndex) {
    return loadAssetDataByMode(groupIndex, entityIndex, MODE_QUEUED_DMA);
}
