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
extern u8 D_800AB090_A2400[];
extern u8 D_8008FE8F_90A8F;
extern s16 D_8008FE8C_90A8C;
extern s32 gPendingDmaCount;
extern Entry D_800A1C20_A2820[];
extern s16 D_800AB078_A23E8[];
extern OSMesgQueue D_800A1820_A2420;
void func_8003AC58_3B858(void*);
extern s32 gDmaQueueIndex;
extern s32 gDmaRequestCount;
extern OSMesg D_800A2128_A2D28[];
extern OSMesgQueue D_800A2110_A2D10;
extern OSMesg gPiDmaMsgBuf[];
extern OSMesgQueue gPiDmaMsgQueue;
extern OSMesgQueue gDmaMsgQueue;
extern OSMesgQueue* D_800A2150_A2D50[];
extern u8 D_800A2168_A2D68[];
extern OSThread D_800A1DC0_A29C0;
extern void func_8003B6B4_3C2B4(void* arg);
extern char piManagerThreadStack[0x8];  // this size seems wrong
extern u8 D_800A1C98_A2898;
extern u8 D_8008FE8E_90A8E;
extern OSPfs controllerPacks[];
extern u8 D_800AFED0_A7240;
extern s32 gControllerPackFileCount;
extern s32 gControllerPackFreeBlockCount;
extern u32 D_800AB080_A23F0[];
extern u8 D_800AB094_A2404[];
extern u8 D_800AB098_A2408[];
extern OSMesgQueue D_800A18A8_A24A8;

typedef struct {
    void* unk0;
    void* unk4;
    s32 unk8;
    s32 unkC;
    void* unk10;
} Entry248C;
extern Entry248C* gDmaQueue;
extern Entry248C* func_8006A258_6AE58(void* arg0, s32 size, void* info);
extern Entry248C* D_800A2494_A3094;
extern Entry248C* D_800A2108_A2D08;

typedef struct {
    u32 unknown;
    u32 gameCode;
    u16 companyCode;
    u8 gameName[16];
    u8 extName[1];  // maybe wrong
} controllerPackFileHeader;

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

void controllerPackDeleteFile(s32 arg0, s32 arg1, controllerPackFileHeader arg2[]) {
    OSPfs* selectedPack;
    s32 controllerID;
    s32 err;
    controllerPackFileHeader* header;
    u16 new_var;
    controllerID = arg0 & 0xFFFF;
    new_var = arg1 & 0xFFFF;
    selectedPack = &controllerPacks[controllerID];

    err = osPfsInitPak(&mainStack, selectedPack, controllerID);
    if (!err) {
        // some kind of 2D array lookup?
        header = ((void*)(((controllerID * 0x10) + new_var) * 0x24)) + ((u32)arg2);

        err = osPfsDeleteFile(selectedPack, header->companyCode, header->gameCode, header->gameName, header->extName);
        if (!err) {
            header->companyCode = 0;
            header->gameCode = 0;
            header->unknown = 0;
        }
    }

    osSendMesg(&D_800A1888_A2488, (void*)err, 1);
}

void func_8003AAC4_3B6C4() {
}

int func_8003AACC_3B6CC() {
    return 0;
}

void controllerPackDeleteFileFromHeader(s32 selectedPack, controllerPackFileHeader* header) {
    OSPfs* selectedControllerPack;
    s32 err;

    selectedControllerPack = &controllerPacks[(u16)selectedPack];
    err = osPfsInitPak(&mainStack, selectedControllerPack, (u16)selectedPack);
    if (!err) {
        err = osPfsDeleteFile(
            selectedControllerPack,
            header->companyCode,
            header->gameCode,
            header->gameName,
            header->extName);

        if (!err) {
            header->companyCode = 0;
            header->gameCode = 0;
        }
    }

    osSendMesg(&D_800A1888_A2488, (void*)err, 1);
}

void func_8003AB74_3B774() {
}

int func_8003AB7C_3B77C() {
    return 0;
}

void controllerPackReadStatus(s32 arg0) {
    s32 sp10;
    s32 temp_a2;
    s32 err;

    temp_a2 = arg0 & 0xFFFF;
    err = osPfsInitPak(&mainStack, &controllerPacks[temp_a2], temp_a2);
    if (err == 0) {
        err = osPfsFreeBlocks(controllerPacks, &gControllerPackFreeBlockCount);
        if (err == 0) {
            err = osPfsNumFiles(controllerPacks, &sp10, &gControllerPackFileCount);
            if (err != 0) {
                gControllerPackFileCount = 0;
            }
        } else {
            gControllerPackFreeBlockCount = 0;
        }
    }

    osSendMesg(&D_800A1888_A2488, (OSMesg*)err, OS_MESG_BLOCK);
}

void func_8003AC30_3B830() {
}

void func_8003AC38_3B838() {
    func_8003AC58_3B858(D_800AB078_A23E8);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AC58_3B858);

void func_8003AF38_3BB38(s32 arg0) {
    s32 temp_v1;
    temp_v1 = arg0 & 0xFFFF;

    if (((s32)D_800AFED0_A7240 >> temp_v1) & 1) {
        D_800AB078_A23E8[temp_v1] = 1;
    }
}

void func_8003AF6C_3BB6C(void) {
    u8* var_v1;
    u8* end;
    u8 a1 = 1;

    var_v1 = (u8*)D_800AB090_A2400;
    end = var_v1 + 4;

    do {
        if (*var_v1 == 0) {
            *var_v1 = a1;
        }
        var_v1++;
    } while ((s32)var_v1 < (s32)end);
}

void func_8003AFA0_3BBA0(void) {
    s32 var_v1;
    int new_var2;
    s32* var_a0;
    s16* var_a1;
    int new_var;
    new_var = 2;
    var_v1 = 0;
    new_var2 = 1;
    var_a1 = D_800AB080_A23F0 - new_var;
    var_a0 = D_800AB080_A23F0;
    do {
        D_800AB090_A2400[var_v1] = 0;
        *var_a0 = new_var2;
        *var_a1 = 0;
        var_a1++;
        D_800AB094_A2404[var_v1] = 0;
        D_800AB098_A2408[var_v1] = 0;
        var_v1++;
        var_a0++;
    } while (var_v1 < 4);
}

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

void* func_8003B098_3BC98(void) {
    void* sp10;
    void* var_v0;

    sp10 = NULL;
    var_v0 = (void*)-1;
    if (osRecvMesg(&D_800A18A8_A24A8, &sp10, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

void func_8003B0E8_3BCE8(void) {
    s16 temp_v0;
    s32 temp_a1;

    D_8008FE8F_90A8F = 1;
    temp_a1 = D_8008FE8C_90A8C;
    D_800A1C20_A2820[temp_a1].command = 0xD0;
    osSendMesg(&D_800A1820_A2420, (OSMesg*)&D_800A1C20_A2820[temp_a1], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void* func_8003B170_3BD70(void) {
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

void func_8003B1C0_3BDC0() {
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromProbe(&mainStack),
        1);
}

void func_8003B1F4_3BDF4(s32 arg0, void* arg1) {
    s16 temp_v0;
    s32 temp_v1;

    D_8008FE8F_90A8F = 1;
    temp_v1 = D_8008FE8C_90A8C;
    D_800A1C20_A2820[temp_v1].arg = arg1;
    D_800A1C20_A2820[temp_v1].command = (arg0 & 0xFF) + 0xE0;
    osSendMesg(&D_800A1820_A2420, (OSMesg*)&D_800A1C20_A2820[temp_v1], 1);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

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

void func_8003B324_3BF24(s32 arg0) {
    s16 temp_v0;
    s32 temp_a1;

    D_8008FE8F_90A8F = 1;
    temp_a1 = D_8008FE8C_90A8C;
    D_800A1C20_A2820[temp_a1].command = (arg0 & 0xFF) + 0xF0;
    osSendMesg(&D_800A1820_A2420, &D_800A1C20_A2820[temp_a1], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void* func_8003B3B0_3BFB0(void) {
    void* sp10;
    void* var_v0;

    sp10 = NULL;
    var_v0 = (void*)-1;
    if (osRecvMesg(&D_800A1888_A2488, &sp10, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

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

void* func_8003B510_3C110(void) {
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

void func_8003B560_3C160(u8* arg0) {
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromLongWrite(&mainStack, 0, arg0, 0x200),
        1);
}

void initPiManager() {
    u8 flag;

    gDmaQueueIndex = 0;
    gDmaRequestCount = 0;
    D_800A2108_A2D08 = func_8006A258_6AE58(0, 0x168, &flag);
    gDmaQueue = func_8006A258_6AE58(0, 0x730, &flag);
    D_800A2494_A3094 = func_8006A258_6AE58(0, 0x1000, &flag);

    osCreatePiManager(150, (OSMesgQueue*)D_800A2150_A2D50, (OSMesg*)D_800A2168_A2D68, 200);
    osCreateMesgQueue(&D_800A2110_A2D10, D_800A2128_A2D28, 1);
    osCreateMesgQueue(&gPiDmaMsgQueue, gPiDmaMsgBuf, 1);
    osCreateMesgQueue(&gDmaMsgQueue, (OSMesg*)D_800A2108_A2D08, 90);

    osCreateThread(&D_800A1DC0_A29C0, 7, func_8003B6B4_3C2B4, 0, &piManagerThreadStack + sizeof(piManagerThreadStack), 1);

    osStartThread(&D_800A1DC0_A29C0);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B6B4_3C2B4);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B8F0_3C4F0);

extern s32 gDmaQueueIndex;
extern OSMesgQueue gDmaMsgQueue;
extern s32 gPendingDmaCount;
extern void func_8006A4DC_6B0DC(void*);
extern void func_8006A524_6B124(void*, s32);

void* dmaQueueRequest(void* romStart, void* romEnd, s32 size) {
    u8 flag;
    Entry248C* entry;
    void* ret;
    s32 a1Val;
    s32 a1;
    (gDmaQueue + gDmaQueueIndex)->unkC = size;
    ret = func_8006A258_6AE58(romStart, size, &flag);
    (gDmaQueue + gDmaQueueIndex)->unk10 = ret;
    if (flag == 0) {
        func_8006A4DC_6B0DC(ret);
        (gDmaQueue + gDmaQueueIndex)->unk0 = romStart;
        entry = gDmaQueue + gDmaQueueIndex;
        entry->unk4 = romEnd;
        entry->unk8 = 1;
        gPendingDmaCount++;
        osSendMesg(&gDmaMsgQueue, entry, 1);
        gDmaQueueIndex++;
        if (gDmaQueueIndex >= 0x5C) {
            gDmaQueueIndex = 0;
        }
        {
            a1 = gDmaRequestCount;
            a1++;
            gDmaRequestCount = a1;
        }
        func_8006A524_6B124(ret, a1);
    }
    return ret;
}

s32 func_8003BB5C_3C75C() {
    return gPendingDmaCount;
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

        osPiStartDma(&dmaMessage, OS_MESG_PRI_NORMAL, OS_READ, (u32)currentRomOffset, currentRamDest, currentChunkSize, &gPiDmaMsgQueue);
        osRecvMesg(&gPiDmaMsgQueue, &dummyMessage, OS_MESG_BLOCK);

        currentRomOffset += currentChunkSize;
        currentRamDest += currentChunkSize;
        remainingBytes -= currentChunkSize;
    }
}
