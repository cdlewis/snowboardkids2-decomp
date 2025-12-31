#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "6E840.h"

typedef struct {
    void *start;
    void *end;
    s32 size;
} Asset_56910;

typedef struct {
    void *start;
    void *end;
} AssetWithoutSize;

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 unk8;
    u8 padding[2];
    Vec3i unkC;      /* 0x0C */
    ColorData unk18; /* 0x18 */
    ColorData unk20; /* 0x20 */
    s16 unk28;       /* 0x28 */
    u8 padding2[0x2];
} D_80090F90_91B90_item;

typedef struct {
    u8 padding[0x40];
    DisplayLists unk40;
    u8 padding2[0x50 - 0x10];
    DisplayLists unk90;
    DisplayLists unkA0;
    DisplayLists unkB0;
    DisplayLists unkC0;
} func_80055E68_56A68_result;

func_80055E68_56A68_result *func_80055E68_56A68(s32);

void *func_80055D34_56934(s32 index);
void *func_80055D7C_5697C(s32 index);
void *func_80055DF8_569F8(s32 index);
void *func_80055DC4_569C4(s32 index);
D_80090F90_91B90_item *func_80055D10_56910(s32 index);
s32 func_80055E40_56A40(s32 arg0);
s32 func_80055E54_56A54(s32 arg0);

void *loadAsset_B7E70(void);
void *loadAsset_216290(void);
void *loadAsset_34CB50(void);
void *loadAsset_34F9A0(void);
void *loadAsset_350140(void);
void *loadAsset_3505F0(void);
void *loadAsset_34F7E0(void);