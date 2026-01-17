#include "common.h"

extern void *getCurrentAllocation(void);
extern void *createSceneModelEx(s32, void *, s8, s8, s8, s16);
extern void setCallback(void *);
extern void initCharSelectBoardPreview(void);

typedef struct {
    void *model;
    char pad4[0x24];
    char playerIndex;
} CharSelectBoardPreview;

extern D_800AFE8C_A71FC_type *D_800AFE8C_A71FC;

void func_80024D40_25940(CharSelectBoardPreview *arg0) {
    void *alloc;
    int index;
    int assetGroupIndex;

    alloc = getCurrentAllocation();
    index = arg0->playerIndex;
    assetGroupIndex = D_800AFE8C_A71FC->unk9[index];

    if (assetGroupIndex == 7) {
        assetGroupIndex = 0x39;
    }

    arg0->model = createSceneModelEx(
        assetGroupIndex,
        (u8 *)alloc + (index * 0x1D8),
        ((s8 *)alloc)[index + 0x18B8],
        -1,
        -1,
        -1
    );
    setCallback(initCharSelectBoardPreview);
}
