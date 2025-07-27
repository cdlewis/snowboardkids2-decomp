#include "common.h"

typedef struct {
    void *start;
    void *end;
    s32 size;
} Asset;

typedef struct {
    void *start;
    void *end;
} AssetWithoutSize;

typedef struct {
    u8 padding[0x2C];
} D_80090F90_91B90_item;

typedef struct {
    u8 padding[0x90];
    s32 unk90;
    s32 padding2[3];
    s32 unkA0;
} func_80055E68_56A68_result;

func_80055E68_56A68_result *func_80055E68_56A68(s32);

void *func_80055D34_56934(s32 index);
void *func_80055DF8_569F8(s32 index);
void *func_80055DC4_569C4(s32 index);
D_80090F90_91B90_item *func_80055D10_56910(s32 index);

void *loadAsset_B7E70(void);
void *loadAsset_216290(void);
void *loadAsset_34CB50(void);
void *loadAsset_34F9A0(void);
void *loadAsset_350140(void);
void *loadAsset_3505F0(void);
void *loadAsset_34F7E0(void);