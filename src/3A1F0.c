#include "common.h"

typedef struct {
    s16 command;
    s16 pad;
    void* arg;
} Entry;

extern OSMesgQueue mainStack;
extern OSMesgQueue D_800A1820_A2420;
extern OSMesgQueue D_800A1888_A2488;
extern OSMesgQueue gPiDmaMsgQueue;
extern u8 D_800AB090_A2400;
extern u8 D_8008FE8F_90A8F;
extern s16 D_8008FE8C_90A8C;
extern s32 D_8008FEA0_90AA0;
extern Entry D_800A1C20_A2820[];
extern u8 D_800AB078_A23E8[];
extern OSMesgQueue D_800A1820_A2420;
void func_8003AC58_3B858(void*);
extern s32 D_800A2488_A3088;
extern s32 D_800A2490_A3090;
extern s32 D_800A2108_A2D08;
extern s32 D_800A248C_A308C;
extern s32 D_800A2494_A3094;
extern OSMesg D_800A2128_A2D28[];
extern OSMesgQueue D_800A2110_A2D10;
extern OSMesg gPiDmaMsgBuf[];
extern OSMesgQueue gPiDmaMsgQueue;
extern OSMesgQueue D_800A20F0_A2CF0;
extern OSMesgQueue* D_800A2150_A2D50[];
extern u8 D_800A2168_A2D68[];
extern OSThread D_800A1DC0_A29C0;
extern void func_8003B6B4_3C2B4(void* arg);
extern s32 func_8006A258_6AE58(s32 arg0, s32 size, void* info);
extern char piManagerThreadStack[0x8];  // this size seems wrong
extern u8 D_800A1C98_A2898;
extern u8 D_8008FE8E_90A8E;
extern OSPfs controllerPacks[];

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_800395F0_3A1F0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_800397CC_3A3CC);

void func_80039B88_3A788(void) {
    s16 temp_v0;
    s32 temp_a1;

    if ((D_8008FE8F_90A8F == 0) && (D_800A1C98_A2898 == 0) && (D_8008FE8E_90A8E != 0)) {
        temp_a1 = D_8008FE8C_90A8C;
        D_800A1C20_A2820[temp_a1].command = 0x10;
        osSendMesg(&D_800A1820_A2420, (OSMesg*)&D_800A1C20_A2820[temp_a1], 1);
        temp_v0 = (u16)D_8008FE8C_90A8C + 1;
        D_8008FE8C_90A8C = temp_v0;
        if (temp_v0 >= 0xF) {
            D_8008FE8C_90A8C = 0;
        }
        D_800A1C98_A2898 = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_80039C34_3A834);

void func_8003A1DC_3ADDC() {
}

int func_8003A1E4_3ADE4() {
    return 0;
}

void initControllerPack(s32 channel) {
    OSMesgQueue* mainStackLocal;
    OSPfs* selectedPack;
    s32 controllerPortNumber;
    s32 controllerPackStatus;
    mainStackLocal = &mainStack;

    controllerPortNumber = channel & 0xFFFF;
    selectedPack = &controllerPacks[controllerPortNumber];

    controllerPackStatus = osPfsInitPak(mainStackLocal, selectedPack, controllerPortNumber);

    if (controllerPackStatus == PFS_ERR_NEW_PACK) {
        controllerPackStatus = osPfsInitPak(mainStackLocal, selectedPack, controllerPortNumber);
    }

    osSendMesg(&D_800A1888_A2488, (OSMesg*)controllerPackStatus, OS_MESG_BLOCK);
}

void func_8003A284_3AE84() {
}

int func_8003A28C_3AE8C() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A294_3AE94);

void func_8003A51C_3B11C() {
}

int func_8003A524_3B124() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A52C_3B12C);

void func_8003A854_3B454() {
}

int func_8003A85C_3B45C() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A864_3B464);

void func_8003A9DC_3B5DC() {
}

int func_8003A9E4_3B5E4() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A9EC_3B5EC);

void func_8003AAC4_3B6C4() {
}

int func_8003AACC_3B6CC() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AAD4_3B6D4);

void func_8003AB74_3B774() {
}

int func_8003AB7C_3B77C() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AB84_3B784);

void func_8003AC30_3B830() {
}

void func_8003AC38_3B838() {
    func_8003AC58_3B858(D_800AB078_A23E8);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AC58_3B858);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AF38_3BB38);

void func_8003AF6C_3BB6C() {
    u8* var_v1;
    u8 a1 = 1;

    var_v1 = &D_800AB090_A2400;
    do {
        if (*var_v1 == 0) {
            *var_v1 = a1;
        }
        var_v1 += 1;
    } while ((s32)var_v1 < (s32)(&D_800AB090_A2400 + 4));
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AFA0_3BBA0);

void func_8003B000_3BC00(s32 arg0) {
    s16 temp_v0;
    s32 temp_a1;
    temp_v0 = arg0 + 0x80;
    temp_a1 = D_8008FE8C_90A8C;
    D_800A1C20_A2820[temp_a1].arg = &D_800AB078_A23E8;
    D_800A1C20_A2820[temp_a1].command = temp_v0;
    osSendMesg(&D_800A1820_A2420, (OSMesg*)(&D_800A1C20_A2820[temp_a1]), 1);
    temp_v0 = ((u16)D_8008FE8C_90A8C) + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
    D_8008FE8F_90A8F = 1;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B098_3BC98);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B0E8_3BCE8);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B170_3BD70);

void func_8003B1C0_3BDC0() {
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromProbe(&mainStack),
        1);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B1F4_3BDF4);

void* func_8003B28C_3BE8C(void) {
    void* sp10;
    void* var_v0;

    sp10 = NULL;
    var_v0 = (void*)-1;
    if (osRecvMesg(&D_800A1888_A2488, &sp10, 0) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

void func_8003B2DC_3BEDC(s32 arg0, u8* arg1) {
    u8 a1 = ((arg0 & 0xFF) * 0x10) & 0xF0;
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromLongRead(&mainStack, a1, arg1, 0x58),
        1);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B324_3BF24);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B3B0_3BFB0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B400_3C000);

void func_8003B47C_3C07C(void* arg0) {
    u32 new_var;
    s16 index;
    s16* addr;

    index = D_8008FE8C_90A8C;
    new_var = index;
    D_8008FE8F_90A8F = 1;
    D_800A1C20_A2820[index].command = 0x140;
    D_800A1C20_A2820[new_var].arg = arg0;

    osSendMesg(&D_800A1820_A2420, &D_800A1C20_A2820[index], OS_MESG_BLOCK);

    index = D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = index;

    if (index >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B510_3C110);

void func_8003B560_3C160(u8* arg0) {
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromLongWrite(&mainStack, 0, arg0, 0x200),
        1);
}

void initPiManager() {
    u8 flag;

    D_800A2488_A3088 = 0;
    D_800A2490_A3090 = 0;
    D_800A2108_A2D08 = func_8006A258_6AE58(0, 0x168, &flag);
    D_800A248C_A308C = func_8006A258_6AE58(0, 0x730, &flag);
    D_800A2494_A3094 = func_8006A258_6AE58(0, 0x1000, &flag);

    osCreatePiManager(150, (OSMesgQueue*)D_800A2150_A2D50, (OSMesg*)D_800A2168_A2D68, 200);
    osCreateMesgQueue(&D_800A2110_A2D10, D_800A2128_A2D28, 1);
    osCreateMesgQueue(&gPiDmaMsgQueue, gPiDmaMsgBuf, 1);
    osCreateMesgQueue(&D_800A20F0_A2CF0, (OSMesg*)D_800A2108_A2D08, 90);

    osCreateThread(&D_800A1DC0_A29C0, 7, func_8003B6B4_3C2B4, 0, &piManagerThreadStack + sizeof(piManagerThreadStack), 1);

    osStartThread(&D_800A1DC0_A29C0);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B6B4_3C2B4);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B8F0_3C4F0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003BA24_3C624);

s32 func_8003BB5C_3C75C() {
    return D_8008FEA0_90AA0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003BB68_3C768);

void func_8003BC58_3C858() {
}

void dmaLoadAndInvalidate(
    u32 romStart,
    s32 romEnd,
    void* ramStart,
    void* icacheStart,
    void* icacheEnd,
    void* dcacheStart,
    void* dcacheEnd,
    void* bssStart,
    void* bssEnd) {
    OSIoMesg dmaMessage;
    void* dummyMessage;
    u32 remainingBytes;
    u32 currentRomOffset;
    u32 currentChunkSize;
    void* currentRamDest;

    // Zero out BSS or other region if requested
    if (bssEnd != bssStart) {
        bzero(bssStart, bssEnd - bssStart);
    }

    // Invalidate instruction and data caches for specified ranges
    osInvalICache(icacheStart, icacheEnd - icacheStart);
    osInvalDCache(dcacheStart, dcacheEnd - dcacheStart);

    remainingBytes = romEnd - romStart;
    currentRomOffset = romStart;
    currentRamDest = ramStart;

    while (remainingBytes > 0) {
        currentChunkSize = remainingBytes;

        // Cap the transfer size to 0x1000 bytes (4KB)
        if (remainingBytes >= 0x1001) {
            currentChunkSize = 0x1000;
        }

        osPiStartDma(&dmaMessage, OS_MESG_PRI_NORMAL, OS_READ, currentRomOffset, currentRamDest, currentChunkSize, &gPiDmaMsgQueue);
        osRecvMesg(&gPiDmaMsgQueue, &dummyMessage, OS_MESG_BLOCK);

        currentRomOffset += currentChunkSize;
        currentRamDest += currentChunkSize;
        remainingBytes -= currentChunkSize;
    }
}
