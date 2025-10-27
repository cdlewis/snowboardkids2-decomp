#include "20F0.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_3ECE40);
USE_ASSET(_40A450);
USE_ASSET(_40A590);
USE_ASSET(_40A760);

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

void *loadAssetByIndex_94F90(s16 groupIndex, s16 pairIndex) {
    AssetPair *assetArray = D_80094390_94F90[groupIndex];
    return dmaRequestAndUpdateState(assetArray[pairIndex].start, assetArray[pairIndex].end);
}

void *loadAssetByIndex_95200(s16 groupIndex, s16 pairIndex) {
    AssetWithSize *assetArray = D_80094600_95200[groupIndex];
    return dmaRequestAndUpdateStateWithSize(
        assetArray[pairIndex].start,
        assetArray[pairIndex].end,
        assetArray[pairIndex].size
    );
}

void *loadAssetByIndex_953E0(s16 index) {
    if (index == 0xA) {
        return NULL;
    } else {
        return dmaRequestAndUpdateStateWithSize(
            D_800947E0_953E0[index].start,
            D_800947E0_953E0[index].end,
            D_800947E0_953E0[index].size
        );
    }
}

s32 loadAssetByIndex_95380(s16 groupIndex, s16 pairIndex) {
    s32 *group = D_80094780_95380[groupIndex];
    return group[pairIndex];
}

s32 loadAssetByIndex_953B0(s16 groupIndex, s16 pairIndex) {
    s32 *group = D_800947B0_953B0[groupIndex];
    return group[pairIndex];
}

void *load_3ECE40(void) {
    return dmaRequestAndUpdateStateWithSize(&_3ECE40_ROM_START, &_3ECE40_ROM_END, 0xA518);
}

void *loadAssetByIndex_95470(s32 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80094870_95470[index].start,
        D_80094870_95470[index].end,
        D_80094870_95470[index].size
    );
}

void *loadAssetByIndex_95500(s16 index) {
    return dmaRequestAndUpdateState(D_80094900_95500[index].start, D_80094900_95500[index].end);
}

void *loadAssetByIndex_95590(s16 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80094990_95590[index].start,
        D_80094990_95590[index].end,
        D_80094990_95590[index].size
    );
}

void *loadAssetByIndex_95668(s16 index) {
    s16 clamped_index;

    if (index == 0x10) {
        return 0;
    }

    clamped_index = index;
    if (index >= 0x10) {
        clamped_index = 0;
    }

    return dmaRequestAndUpdateStateWithSize(
        D_80094A68_95668[clamped_index].start,
        D_80094A68_95668[clamped_index].end,
        D_80094A68_95668[clamped_index].size
    );
}

void *loadAssetByIndex_95728(s16 index) {
    return D_80094B28_95728[index];
}

void *func_8005DC60_5E860(s32 index) {
    void *start;
    void *end;
    s32 size;
    u8 val = ((GameState *)getCurrentAllocation())->unk7A;

    if (val == 1) {
        return dmaRequestAndUpdateStateWithSize(&_40A450_ROM_START, &_40A450_ROM_END, 0x250);
    } else if (val == 2) {
        return dmaRequestAndUpdateStateWithSize(&_40A590_ROM_START, &_40A590_ROM_END, 0x260);
    } else if (val == 3) {
        return dmaRequestAndUpdateStateWithSize(&_40A760_ROM_START, &_40A760_ROM_END, 0x270);
    } else {
        return dmaRequestAndUpdateStateWithSize(
            D_80094B70_95770[index].start,
            D_80094B70_95770[index].end,
            D_80094B70_95770[index].size
        );
    }
}

s32 loadAssetByIndex_95770(s32 index) {
    GameState *state = (GameState *)getCurrentAllocation();
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

void *loadAssetByIndex_5E990(s32 index) {
    return dmaRequestAndUpdateStateWithSize(
        D_80094C30_95830[index].start,
        D_80094C30_95830[index].end,
        D_80094C30_95830[index].size
    );
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
