#include "3A1F0.h"

#include "EepromSaveData_type.h"
#include "memory_allocator.h"

typedef struct {
    s16 command;
    u16 pad;
    void *arg;
} Entry;

typedef enum {
    DMA_COMPRESSION_NONE = 0,
    DMA_COMPRESSION_RLE = 1,
    DMA_COMPRESSION_DIRECT_BUFFER = 2
} DmaCompressionType;

typedef struct {
    void *start;
    void *end;
    s32 compressionType;
    s32 size;
    void *dramAddr;
} DmaTransferEntry;

typedef struct {
    s32 fileSize;
    u32 gameCode;
    u16 companyCode;
    u8 gameName[16];
    u8 extName[4];
    u8 padding[6];
} controllerPackFileHeader;

typedef struct {
    s16 unk0[4];
    u32 unk8[4];
    u8 unk18[4];
    u8 unk1C[4];
    u8 unk20[4];
} D_800AB078_A23E8_type;

extern OSMesgQueue D_800A1820_A2420;
extern OSMesgQueue D_800A1868_A2468;
extern OSMesgQueue D_800A1888_A2488;
extern OSMesgQueue D_800A18A8_A24A8;
extern OSMesgQueue mainStack;
extern OSPfs D_800A1A68_A2668[];
extern OSContPad D_800A1C08_A2808[];

void initControllerPack(s32);
void func_8003A294_3AE94(s32, void *);
void func_8003A52C_3B12C(s32, void *);
void func_8003A864_3B464(s32, controllerPackFileHeader *);
void controllerPackDeleteFile(s32 arg0, s32 arg1, controllerPackFileHeader arg2[]);
void controllerPackDeleteFileFromHeader(s32 selectedPack, controllerPackFileHeader *header);
void controllerPackReadStatus(s32 arg0);
void func_8003B1C0_3BDC0(void);
void func_8003B2DC_3BEDC(s32 arg0, u8 *arg1);
void func_8003B400_3C000(s32);
void func_8003B560_3C160(u8 *);
void func_800397CC_3A3CC(void *arg0);

extern char piManagerThreadStack[0x8]; // this size seems wrong
extern DmaTransferEntry *D_800A2108_A2D08;
extern DmaTransferEntry *gDmaQueue;
extern Entry D_800A1C20_A2820[];
extern OSMesg D_800A2128_A2D28[];
extern OSMesg gPiDmaMsgBuf[];

extern OSMesgQueue D_800A2110_A2D10;
extern OSMesgQueue gDmaMsgQueue;
extern OSMesgQueue gPiDmaMsgQueue;

extern OSMesgQueue *D_800A2150_A2D50[];
extern OSPfs controllerPacks[];
extern OSThread D_800A1DC0_A29C0;
extern s16 D_8008FE8C_90A8C;
extern D_800AB078_A23E8_type D_800AB078_A23E8;
extern s32 gControllerPackFileCount;
extern s32 gControllerPackFreeBlockCount;
extern s32 gDmaQueueIndex;
extern s32 gDmaRequestCount;
extern s32 gPendingDmaCount;
extern u32 D_800AB080_A23F0[];
extern u8 D_8008FE8E_90A8E;
extern u8 D_8008FE8F_90A8F;
extern u8 D_800A1C98_A2898;
extern u8 D_800A2168_A2D68[];
extern u8 D_800AB090_A2400[];
extern u8 D_800AB094_A2404[];
extern u8 D_800AB098_A2408[];
extern u8 gMotorInitCompleteMask;
extern u8 *gDmaCompressionBuffer;

void piDmaHandlerThread(void *);
void func_8003AC58_3B858(D_800AB078_A23E8_type *);

void func_8003AFA0_3BBA0(void);
extern OSContStatus D_8009F660_A0260;
extern OSThread D_8009F670_A0270;
extern s32 D_800A1838_A2438;

extern s32 D_800A1880_A2480;
extern s32 D_800A18A0_A24A0;
extern s32 D_800A18C0_A24C0;
extern s32 D_800A8D10_A0080;
extern u8 gConnectedControllerMask;
extern s8 gControllerPollingEnabled;

void initControllerSubsystem(void) {
    s32 result;
    s32 i;
    u8 controller_status;

    osCreateMesgQueue(&mainStack, (OSMesg)&D_800A8D10_A0080, 1);
    osCreateMesgQueue(&D_800A1820_A2420, (OSMesg)&D_800A1838_A2438, 0xC);
    osCreateMesgQueue(&D_800A1868_A2468, (OSMesg)&D_800A1880_A2480, 1);
    osCreateMesgQueue(&D_800A1888_A2488, (OSMesg)&D_800A18A0_A24A0, 1);
    osCreateMesgQueue(&D_800A18A8_A24A8, (OSMesg)&D_800A18C0_A24C0, 1);
    osSetEventMesg(5, &mainStack, (OSMesg)1);

    for (i = 0; i < 4; i++) {
        result = osContInit(&mainStack, &D_8008FE8E_90A8E, &D_8009F660_A0260);
        gConnectedControllerMask = 0;
        if (result == 0) {
            controller_status = D_8008FE8E_90A8E;

            // Find the first disconnected controller, if any
            for (i = 0; i < 4; i++) {
                if (!((controller_status >> i) & 1)) {
                    break;
                }
                gConnectedControllerMask |= (1 << i);
            }

            for (i = 0; i < 4; i++) {
                D_800A1C08_A2808[i].button = 0;
                D_800A1C08_A2808[i].stick_x = 0;
                D_800A1C08_A2808[i].stick_y = 0;
            }

            D_800A1C98_A2898 = 0;
            func_8003AFA0_3BBA0();
            gControllerPollingEnabled = 1;
            osCreateThread(&D_8009F670_A0270, 6, &func_800397CC_3A3CC, 0, &D_800A1820_A2420, 3);
            osStartThread(&D_8009F670_A0270);
            return;
        }

        D_800A1C98_A2898 = 0;
        gControllerPollingEnabled = 1;
    }
}

void func_800397CC_3A3CC(void *arg0) {
    OSPfs *motorPfs;
    D_800AB078_A23E8_type *motorArg;
    Entry *msg;
    OSMesg mesg2;
    char channel;
    s16 cmd;
    OSMesgQueue *queue;

    msg = NULL;
    while (TRUE) {
        osRecvMesg(&D_800A1820_A2420, (OSMesg *)(&msg), 1);
        cmd = msg->command;
        switch (cmd & 0x1F0) {
            case 0x10:
                osContStartReadData(&mainStack);
                osRecvMesg(&mainStack, &mesg2, 1);
                osContGetReadData(D_800A1C08_A2808);
                osSendMesg(&D_800A1868_A2468, (OSMesg)1, 0);
                continue;

            case 0x20:
                initControllerPack(cmd & 3);
                continue;

            case 0x30:
                func_8003A294_3AE94(cmd & 3, msg->arg);
                continue;

            case 0x40:
                func_8003A52C_3B12C(cmd & 3, msg->arg);
                continue;

            case 0x50:
                func_8003A864_3B464(cmd & 3, msg->arg);
                continue;

            case 0x60:
                controllerPackDeleteFile(cmd & 3, msg->pad, msg->arg);
                continue;

            case 0xC0:
                controllerPackDeleteFileFromHeader(cmd & 3, msg->arg);
                continue;

            case 0x70:
                controllerPackReadStatus(cmd & 3);
                continue;

            case 0x80:
                motorArg = msg->arg;
                channel = cmd & 3;
                motorArg->unk8[msg->command & 3] = osMotorInit(&mainStack, &D_800A1A68_A2668[channel], channel);
                osSendMesg(&D_800A18A8_A24A8, (OSMesg)motorArg->unk8[msg->command & 3], 0);
                continue;

            case 0x90:
                motorArg = msg->arg;
                queue = &mainStack;
                channel = cmd & 3;
                motorArg->unk8[msg->command & 3] = osMotorInit(queue, &D_800A1A68_A2668[channel], channel);
                continue;

            case 0xA0:
                motorArg = msg->arg;
                channel = cmd & 3;
                if (motorArg->unk8[channel] != 0) {
                    queue = &mainStack;
                    motorArg->unk8[msg->command & 3] = osMotorInit(queue, &D_800A1A68_A2668[channel], channel);
                    continue;
                }
                (*motorArg).unk8[msg->command & 3] = osMotorStart(&D_800A1A68_A2668[channel]);
                continue;

            case 0xB0:
                motorArg = msg->arg;
                channel = cmd & 3;
                if (motorArg->unk8[channel] == 0) {
                    motorArg->unk8[msg->command & 3] = osMotorStop(&D_800A1A68_A2668[channel]);
                    continue;
                }
                queue = &mainStack;
                motorArg->unk8[msg->command & 3] = osMotorInit(queue, &D_800A1A68_A2668[channel], channel);
                continue;

            case 0xD0:
                func_8003B1C0_3BDC0();
                continue;

            case 0xE0:
                func_8003B2DC_3BEDC(cmd & 3, msg->arg);
                continue;

            case 0xF0:
                func_8003B400_3C000(cmd & 3);
                continue;

            case 0x140:
                func_8003B560_3C160(msg->arg);
                continue;
        }
    }
}

void func_80039B88_3A788(void) {
    s16 temp_v0;
    s32 temp_a1;

    if ((D_8008FE8F_90A8F == 0) && (D_800A1C98_A2898 == 0) && (D_8008FE8E_90A8E != 0)) {
        temp_a1 = D_8008FE8C_90A8C;
        D_800A1C20_A2820[temp_a1].command = 0x10;
        osSendMesg(&D_800A1820_A2420, (OSMesg *)&D_800A1C20_A2820[temp_a1], 1);
        temp_v0 = (u16)D_8008FE8C_90A8C + 1;
        D_8008FE8C_90A8C = temp_v0;
        if (temp_v0 >= 0xF) {
            D_8008FE8C_90A8C = 0;
        }
        D_800A1C98_A2898 = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_80039C34_3A834);

void func_8003A1DC_3ADDC(void) {
}

int func_8003A1E4_3ADE4(void) {
    return 0;
}

void initControllerPack(s32 channel) {
    OSMesgQueue *mainStackLocal;
    OSPfs *selectedPack;
    s32 controllerPortNumber;
    s32 controllerPackStatus;
    mainStackLocal = &mainStack;

    controllerPortNumber = channel & 0xFFFF;
    selectedPack = &controllerPacks[controllerPortNumber];

    controllerPackStatus = osPfsInitPak(mainStackLocal, selectedPack, controllerPortNumber);

    if (controllerPackStatus == PFS_ERR_NEW_PACK) {
        controllerPackStatus = osPfsInitPak(mainStackLocal, selectedPack, controllerPortNumber);
    }

    osSendMesg(&D_800A1888_A2488, (OSMesg *)controllerPackStatus, OS_MESG_BLOCK);
}

void func_8003A284_3AE84(void) {
}

int func_8003A28C_3AE8C(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A294_3AE94);

void func_8003A51C_3B11C(void) {
}

int func_8003A524_3B124(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A52C_3B12C);

void func_8003A854_3B454(void) {
}

int func_8003A85C_3B45C(void) {
    return 0;
}

void func_8003A864_3B464(s32 arg0, controllerPackFileHeader *arg1) {
    OSPfsState pfsState;
    s32 controllerPort;
    s32 var_v0;
    s32 i;
    s32 baseIndex;
    s32 j;
    u8 *gameName;
    u8 *extName;
    s32 packsOffset;

    controllerPort = arg0 & 0xFFFF;
    var_v0 = osPfsInitPak(&mainStack, &controllerPacks[controllerPort], controllerPort);
    if (var_v0 == 0) {
        i = 0;
        packsOffset = (s32)&controllerPacks[controllerPort] - (s32)controllerPacks;
        gameName = (u8 *)pfsState.game_name;
        extName = (u8 *)pfsState.ext_name;
        baseIndex = controllerPort << 4;
        do {
            if (osPfsFileState((OSPfs *)((s32)controllerPacks + packsOffset), i, &pfsState) != 0) {
                arg1[baseIndex + i].gameCode = 0;
                arg1[baseIndex + i].companyCode = 0;
            } else {
                arg1[baseIndex + i].fileSize = pfsState.file_size;
                j = 0;
                arg1[baseIndex + i].gameCode = pfsState.game_code;
                arg1[baseIndex + i].companyCode = pfsState.company_code;
                do {
                    arg1[baseIndex + i].gameName[j] = gameName[j];
                    j++;
                } while (j < 16);
                j = 0;
                do {
                    arg1[baseIndex + i].extName[j] = extName[j];
                    j++;
                } while (j < 4);
            }
            i++;
        } while (i < 16);
        var_v0 = 0;
    }
    osSendMesg(&D_800A1888_A2488, (OSMesg)var_v0, 1);
}

void func_8003A9DC_3B5DC(void) {
}

int func_8003A9E4_3B5E4(void) {
    return 0;
}

void controllerPackDeleteFile(s32 arg0, s32 arg1, controllerPackFileHeader arg2[]) {
    OSPfs *selectedPack;
    s32 controllerID;
    s32 err;
    controllerPackFileHeader *header;
    u16 new_var;
    controllerID = arg0 & 0xFFFF;
    new_var = arg1 & 0xFFFF;
    selectedPack = &controllerPacks[controllerID];

    err = osPfsInitPak(&mainStack, selectedPack, controllerID);
    if (!err) {
        // some kind of 2D array lookup?
        header = ((void *)(((controllerID * 0x10) + new_var) * 0x24)) + ((u32)arg2);

        err = osPfsDeleteFile(selectedPack, header->companyCode, header->gameCode, header->gameName, header->extName);
        if (!err) {
            header->companyCode = 0;
            header->gameCode = 0;
            header->fileSize = 0;
        }
    }

    osSendMesg(&D_800A1888_A2488, (void *)err, OS_MESG_BLOCK);
}

void func_8003AAC4_3B6C4(void) {
}

int func_8003AACC_3B6CC(void) {
    return 0;
}

void controllerPackDeleteFileFromHeader(s32 selectedPack, controllerPackFileHeader *header) {
    OSPfs *selectedControllerPack;
    s32 err;

    selectedControllerPack = &controllerPacks[(u16)selectedPack];
    err = osPfsInitPak(&mainStack, selectedControllerPack, (u16)selectedPack);
    if (!err) {
        err = osPfsDeleteFile(
            selectedControllerPack,
            header->companyCode,
            header->gameCode,
            header->gameName,
            header->extName
        );

        if (!err) {
            header->companyCode = 0;
            header->gameCode = 0;
        }
    }

    osSendMesg(&D_800A1888_A2488, (void *)err, OS_MESG_BLOCK);
}

void func_8003AB74_3B774(void) {
}

int func_8003AB7C_3B77C(void) {
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

    osSendMesg(&D_800A1888_A2488, (OSMesg *)err, OS_MESG_BLOCK);
}

void func_8003AC30_3B830(void) {
}

void func_8003AC38_3B838(void) {
    func_8003AC58_3B858(&D_800AB078_A23E8);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AC58_3B858);

void func_8003AF38_3BB38(s32 arg0) {
    s32 temp_v1;
    temp_v1 = arg0 & 0xFFFF;

    if (((s32)gMotorInitCompleteMask >> temp_v1) & 1) {
        D_800AB078_A23E8.unk0[temp_v1] = 1;
    }
}

void func_8003AF6C_3BB6C(void) {
    u8 *var_v1;
    u8 *end;
    u8 a1 = 1;
    s32 i;

    var_v1 = (u8 *)D_800AB078_A23E8.unk18;
    end = var_v1 + 4;

    do {
        if (*var_v1 == 0) {
            *var_v1 = a1;
        }
        var_v1++;
    } while ((s32)var_v1 < (s32)end);
}

void func_8003AFA0_3BBA0(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800AB078_A23E8.unk18[i] = 0;
        D_800AB078_A23E8.unk8[i] = 1;
        D_800AB078_A23E8.unk0[i] = 0;
        D_800AB078_A23E8.unk1C[i] = 0;
        D_800AB078_A23E8.unk20[i] = 0;
    }
}

void func_8003B000_3BC00(s32 arg0) {
    s16 temp_v0;
    s32 temp_a1;
    temp_v0 = arg0 + 0x80;
    temp_a1 = D_8008FE8C_90A8C;
    D_800A1C20_A2820[temp_a1].arg = &D_800AB078_A23E8;
    D_800A1C20_A2820[temp_a1].command = temp_v0;
    osSendMesg(&D_800A1820_A2420, (OSMesg *)(&D_800A1C20_A2820[temp_a1]), OS_MESG_BLOCK);
    temp_v0 = ((u16)D_8008FE8C_90A8C) + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
    D_8008FE8F_90A8F = 1;
}

void *func_8003B098_3BC98(void) {
    void *sp10;
    void *var_v0;

    sp10 = NULL;
    var_v0 = (void *)-1;
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
    osSendMesg(&D_800A1820_A2420, (OSMesg *)&D_800A1C20_A2820[temp_a1], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void *func_8003B170_3BD70(void) {
    void *sp10;
    void *var_v0;

    sp10 = NULL;
    var_v0 = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &sp10, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

void func_8003B1C0_3BDC0(void) {
    osSendMesg(&D_800A1888_A2488, (OSMesg *)osEepromProbe(&mainStack), OS_MESG_BLOCK);
}

void func_8003B1F4_3BDF4(s32 arg0, void *arg1) {
    s16 temp_v0;
    s32 temp_v1;

    D_8008FE8F_90A8F = 1;
    temp_v1 = D_8008FE8C_90A8C;
    D_800A1C20_A2820[temp_v1].arg = arg1;
    D_800A1C20_A2820[temp_v1].command = (arg0 & 0xFF) + 0xE0;
    osSendMesg(&D_800A1820_A2420, (OSMesg *)&D_800A1C20_A2820[temp_v1], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void *func_8003B28C_3BE8C(void) {
    void *sp10;
    void *var_v0;

    sp10 = NULL;
    var_v0 = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &sp10, 0) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

void func_8003B2DC_3BEDC(s32 arg0, u8 *arg1) {
    u8 a1 = ((arg0 & 0xFF) * 0x10) & 0xF0;
    osSendMesg(&D_800A1888_A2488, (OSMesg *)osEepromLongRead(&mainStack, a1, arg1, 0x58), OS_MESG_BLOCK);
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

void *func_8003B3B0_3BFB0(void) {
    void *sp10;
    void *var_v0;

    sp10 = NULL;
    var_v0 = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &sp10, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

void func_8003B400_3C000(s32 arg0) {
    u8 *ptr;
    int new_var2;
    s32 limit;
    s32 sum;
    s32 temp;
    s32 counter;
    s32 ret;
    ptr = EepromSaveData->header_data;
    new_var2 = 4;
    limit = 0x58;
    sum = 0;

    EepromSaveData->checksum = 0;

    for (counter = 0; counter < limit; counter++) {
        temp = *ptr;
        sum += temp;
        ptr++;
    }

    ret = (u8)arg0;
    ret = ret << new_var2;
    counter = ret;
    EepromSaveData->checksum = sum;
    ret = osEepromLongWrite(&mainStack, counter & 0xF0, (u8 *)EepromSaveData, limit);
    osSendMesg(&D_800A1888_A2488, (OSMesg)ret, 1);
}

void func_8003B47C_3C07C(void *arg0) {
    u32 new_var;
    s16 index;
    s16 *addr;

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

void *func_8003B510_3C110(void) {
    void *sp10;
    void *var_v0;

    sp10 = NULL;
    var_v0 = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &sp10, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        var_v0 = sp10;
    }
    return var_v0;
}

void func_8003B560_3C160(u8 *arg0) {
    osSendMesg(&D_800A1888_A2488, (OSMesg *)osEepromLongWrite(&mainStack, 0, arg0, 0x200), OS_MESG_BLOCK);
}

void initPiManager(void) {
    u8 flag;

    gDmaQueueIndex = 0;
    gDmaRequestCount = 0;
    D_800A2108_A2D08 = allocateMemoryNode(0, 0x168, &flag);
    gDmaQueue = allocateMemoryNode(0, 0x730, &flag);
    gDmaCompressionBuffer = allocateMemoryNode(0, 0x1000, &flag);

    osCreatePiManager(150, (OSMesgQueue *)D_800A2150_A2D50, (OSMesg *)D_800A2168_A2D68, 200);
    osCreateMesgQueue(&D_800A2110_A2D10, D_800A2128_A2D28, 1);
    osCreateMesgQueue(&gPiDmaMsgQueue, gPiDmaMsgBuf, 1);
    osCreateMesgQueue(&gDmaMsgQueue, (OSMesg *)D_800A2108_A2D08, 90);

    osCreateThread(
        &D_800A1DC0_A29C0,
        7,
        piDmaHandlerThread,
        0,
        &piManagerThreadStack + sizeof(piManagerThreadStack),
        1
    );

    osStartThread(&D_800A1DC0_A29C0);
}

void piDmaHandlerThread(void *arg __attribute__((unused))) {
    OSIoMesg dmaMsg;
    DmaTransferEntry *entry;
    OSMesg dummyMesg;
    s32 dstOffset;
    s32 copySize;
    s32 size;
    s32 srcOffset;
    s32 copyCount;
    u32 devAddr;
    u8 *dramAddr;
    u8 *dst;
    u8 *src;
    u8 *ptr;
    u8 byte1;
    s32 count;
    s16 new_var;

    while (TRUE) {
        osRecvMesg(&gDmaMsgQueue, (OSMesg)&entry, OS_MESG_BLOCK);

        dstOffset = 0;

        if (entry->compressionType == DMA_COMPRESSION_NONE) {
            size = entry->size;
            osInvalDCache(entry->dramAddr, size);
            devAddr = (u32)entry->start;
            dramAddr = entry->dramAddr;

            while (size != 0) {
                copySize = size;
                if ((u32)size >= 0x1001U) {
                    copySize = 0x1000;
                }

                osPiStartDma(&dmaMsg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, dramAddr, copySize, &D_800A2110_A2D10);
                osRecvMesg(&D_800A2110_A2D10, &dummyMesg, OS_MESG_BLOCK);

                devAddr += copySize;
                size -= copySize;
                dramAddr += copySize;
            }
        } else {
            dramAddr = entry->dramAddr;
            size = entry->end - entry->start;
            devAddr = (u32)entry->start;

            while (size != 0) {
                volatile u8 padding[0x10];
                osInvalDCache(gDmaCompressionBuffer, 0x1000);
                new_var = 0;
                copySize = size;
                if ((u32)size >= 0x1001U) {
                    copySize = 0x1000;
                }

                osPiStartDma(
                    &dmaMsg,
                    OS_MESG_PRI_NORMAL,
                    OS_READ,
                    devAddr,
                    gDmaCompressionBuffer,
                    copySize,
                    &D_800A2110_A2D10
                );
                osRecvMesg(&D_800A2110_A2D10, &dummyMesg, 1);

                srcOffset = 0;
                if (copySize > 0) {
                    dst = (u8 *)((u32)dstOffset + (u32)dramAddr);

                    do {
                        s32 temp_count;
                        s32 temp_offset;

                        src = gDmaCompressionBuffer + srcOffset;
                        byte1 = src[0];
                        count = byte1 & 0xFF;

                        if (count == 0) {
                            *dst = src[1];
                            dstOffset++;
                            dst++;
                        } else {
                            temp_offset = byte1 & 0xF;
                            temp_offset = temp_offset << 8;
                            temp_offset |= src[1];

                            copyCount = 0;
                            temp_count = (u32)count >> 4;

                            if (temp_count != 0) {
                                do {
                                    ptr = (u8 *)(((u32)dstOffset) + ((u32)dramAddr));
                                    do {
                                        dst++;
                                        *ptr = dramAddr[dstOffset - temp_offset];
                                        dstOffset++;
                                        copyCount++;
                                        ptr++;
                                    } while (copyCount < temp_count);
                                } while (0);
                            }
                        }

                        srcOffset += 2;

                        if ((u32)dstOffset >= entry->size) {
                            goto END;
                        }

                    } while (srcOffset < copySize);
                }

                size -= copySize;
                devAddr += copySize;
            }
        }

    END:
        unlockNodeWithInterruptDisable((s32 *)entry->dramAddr);

        if ((u32)entry->compressionType < DMA_COMPRESSION_DIRECT_BUFFER) {
            gPendingDmaCount--;
        }
    }
}

void *queueUncompressedDmaTransfer(void *start, void *end) {
    void *dramAddr;
    u8 nodeAlreadyExists;
    s32 size;
    size = (s32)end - (s32)start;

    gDmaQueue[gDmaQueueIndex].size = size;

    dramAddr = allocateMemoryNode((s32)start, size, &nodeAlreadyExists);

    gDmaQueue[gDmaQueueIndex].dramAddr = dramAddr;

    if (nodeAlreadyExists) {
        return dramAddr;
    }

    markNodeAsLocked(dramAddr);

    gDmaQueue[gDmaQueueIndex].start = start;
    gDmaQueue[gDmaQueueIndex].end = end;
    gDmaQueue[gDmaQueueIndex].compressionType = DMA_COMPRESSION_NONE;

    gPendingDmaCount++;
    osSendMesg(&gDmaMsgQueue, (OSMesg)&gDmaQueue[gDmaQueueIndex], OS_MESG_BLOCK);
    if ((++gDmaQueueIndex) >= 0x5C) {
        gDmaQueueIndex = 0;
    }

    gDmaRequestCount++;

    setNodeSequenceNumber(dramAddr, (void *)gDmaRequestCount);

    return dramAddr;
}

void *queueCompressedDmaTransfer(void *romStart, void *romEnd, s32 size) {
    u8 flag;
    DmaTransferEntry *entry;
    MemoryAllocatorNode *allocatedSpaceStart;
    s32 a1Val;
    s32 a1;

    gDmaQueue[gDmaQueueIndex].size = size;
    allocatedSpaceStart = allocateMemoryNode((s32)romStart, size, &flag);
    gDmaQueue[gDmaQueueIndex].dramAddr = allocatedSpaceStart;

    if (flag == 0) {
        markNodeAsLocked(allocatedSpaceStart);
        gDmaQueue[gDmaQueueIndex].start = romStart;
        entry = &gDmaQueue[gDmaQueueIndex];
        entry->end = romEnd;
        entry->compressionType = DMA_COMPRESSION_RLE;
        gPendingDmaCount++;
        osSendMesg(&gDmaMsgQueue, entry, OS_MESG_BLOCK);
        gDmaQueueIndex++;
        if (gDmaQueueIndex >= 0x5C) {
            gDmaQueueIndex = 0;
        }
        {
            a1 = gDmaRequestCount;
            a1++;
            gDmaRequestCount = a1;
        }
        setNodeSequenceNumber(allocatedSpaceStart, (void *)a1);
    }

    return allocatedSpaceStart;
}

s32 func_8003BB5C_3C75C(void) {
    return gPendingDmaCount;
}

s32 *queueDirectDmaTransfer(void *romStart, void *romEnd, s32 size, s32 *dramAddr) {
    (&gDmaQueue[gDmaQueueIndex])->size = size;
    (&gDmaQueue[gDmaQueueIndex])->dramAddr = dramAddr;

    markNodeAsLocked(dramAddr);

    (&gDmaQueue[gDmaQueueIndex])->start = romStart;
    (&gDmaQueue[gDmaQueueIndex])->end = romEnd;
    (&gDmaQueue[gDmaQueueIndex])->compressionType = DMA_COMPRESSION_DIRECT_BUFFER;

    osSendMesg(&gDmaMsgQueue, &gDmaQueue[gDmaQueueIndex], 1);

    if (++gDmaQueueIndex >= 0x5C) {
        gDmaQueueIndex = 0;
    }
    gDmaRequestCount++;

    setNodeSequenceNumber(dramAddr, (void *)gDmaRequestCount);

    return dramAddr;
}

void func_8003BC58_3C858(void) {
}

void dmaLoadAndInvalidate(
    void *romStart,
    void *romEnd,
    void *ramStart,
    void *icacheStart,
    void *icacheEnd,
    void *dcacheStart,
    void *dcacheEnd,
    void *bssStart,
    void *bssEnd
) {
    OSIoMesg dmaMessage;
    void *dummyMessage;
    u32 remainingBytes;
    void *currentRomOffset;
    u32 currentChunkSize;
    void *currentRamDest;

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

        osPiStartDma(
            &dmaMessage,
            OS_MESG_PRI_NORMAL,
            OS_READ,
            (u32)currentRomOffset,
            currentRamDest,
            currentChunkSize,
            &gPiDmaMsgQueue
        );
        osRecvMesg(&gPiDmaMsgQueue, &dummyMessage, OS_MESG_BLOCK);

        currentRomOffset += currentChunkSize;
        currentRamDest += currentChunkSize;
        remainingBytes -= currentChunkSize;
    }
}
