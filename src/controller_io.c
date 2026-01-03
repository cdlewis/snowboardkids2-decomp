#include "controller_io.h"

#include "EepromSaveData_type.h"
#include "memory_allocator.h"

typedef struct {
    s16 command;
    u16 pad;
    void *arg;
} Entry;

typedef struct {
    s16 intensity[4];
    u32 status[4];
    u8 state[4];
    u8 unk1C[4];
    u8 duration[4];
} MotorState;

void initControllerPack(s32);
void func_8003A294_3AE94(s32, void *);
void func_8003A52C_3B12C(s32, void *);
void controllerPackListFiles(s32 channel, controllerPackFileHeader *fileHeaders);
void controllerPackDeleteFile(s32 arg0, s32 arg1, controllerPackFileHeader arg2[]);
void controllerPackDeleteFileFromHeader(s32 selectedPack, controllerPackFileHeader *header);
void controllerPackReadStatus(s32 arg0);
void eepromProbe(void);
void eepromRead(s32 slotIndex, u8 *buffer);
void eepromWrite(s32);
void eepromWriteAll(u8 *buffer);
void controllerServiceThread(void *arg0);
void motorProcessState(MotorState *);
void initMotorStates(void);

// Data
extern u8 D_8008FE8F_90A8F;
extern u8 D_8008FE8E_90A8E;
extern s16 D_8008FE8C_90A8C;

// Bss
extern OSMesgQueue D_800A1820_A2420;
extern OSMesgQueue D_800A1868_A2468;
extern OSMesgQueue D_800A1888_A2488;
extern OSMesgQueue D_800A18A8_A24A8;
extern OSMesgQueue mainStack;
extern OSPfs D_800A1A68_A2668[];
extern OSContPad D_800A1C08_A2808[];
extern s32 D_800A1880_A2480;
extern s32 D_800A18A0_A24A0;
extern s32 D_800A18C0_A24C0;
extern s32 D_800A8D10_A0080;
extern u8 gConnectedControllerMask;
extern s8 gControllerPollingEnabled;
extern OSContStatus D_8009F660_A0260;
extern OSThread D_8009F670_A0270;
extern s32 D_800A1838_A2438;
extern u8 D_800A1C98_A2898;
extern u8 gMotorInitCompleteMask;
extern MotorState gMotorState;
extern s32 gControllerPackFileCount;
extern s32 gControllerPackFreeBlockCount;
extern OSPfs controllerPacks[];
extern OSThread D_800A1DC0_A29C0;
extern Entry D_800A1C20_A2820[];

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
            initMotorStates();
            gControllerPollingEnabled = 1;
            osCreateThread(&D_8009F670_A0270, 6, &controllerServiceThread, 0, &D_800A1820_A2420, 3);
            osStartThread(&D_8009F670_A0270);
            return;
        }

        D_800A1C98_A2898 = 0;
        gControllerPollingEnabled = 1;
    }
}

void controllerServiceThread(void *arg0) {
    OSPfs *motorPfs;
    MotorState *motorState;
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
                controllerPackListFiles(cmd & 3, msg->arg);
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
                motorState = msg->arg;
                channel = cmd & 3;
                motorState->status[msg->command & 3] = osMotorInit(&mainStack, &D_800A1A68_A2668[channel], channel);
                osSendMesg(&D_800A18A8_A24A8, (OSMesg)motorState->status[msg->command & 3], 0);
                continue;

            case 0x90:
                motorState = msg->arg;
                queue = &mainStack;
                channel = cmd & 3;
                motorState->status[msg->command & 3] = osMotorInit(queue, &D_800A1A68_A2668[channel], channel);
                continue;

            case 0xA0:
                motorState = msg->arg;
                channel = cmd & 3;
                if (motorState->status[channel] != 0) {
                    queue = &mainStack;
                    motorState->status[msg->command & 3] = osMotorInit(queue, &D_800A1A68_A2668[channel], channel);
                    continue;
                }
                (*motorState).status[msg->command & 3] = osMotorStart(&D_800A1A68_A2668[channel]);
                continue;

            case 0xB0:
                motorState = msg->arg;
                channel = cmd & 3;
                if (motorState->status[channel] == 0) {
                    motorState->status[msg->command & 3] = osMotorStop(&D_800A1A68_A2668[channel]);
                    continue;
                }
                queue = &mainStack;
                motorState->status[msg->command & 3] = osMotorInit(queue, &D_800A1A68_A2668[channel], channel);
                continue;

            case 0xD0:
                eepromProbe();
                continue;

            case 0xE0:
                eepromRead(cmd & 3, msg->arg);
                continue;

            case 0xF0:
                eepromWrite(cmd & 3);
                continue;

            case 0x140:
                eepromWriteAll(msg->arg);
                continue;
        }
    }
}

void startControllerRead(void) {
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

INCLUDE_ASM("asm/nonmatchings/controller_io", func_80039C34_3A834);

void controllerPackInitAsyncStub(void) {
}

int controllerPackInitPollStub(void) {
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

void controllerPackReadAsyncStub(void) {
}

int controllerPackReadPollStub(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/controller_io", func_8003A294_3AE94);

void controllerPackWriteAsyncStub(void) {
}

int controllerPackWritePollStub(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/controller_io", func_8003A52C_3B12C);

void controllerPackListFilesAsyncStub(void) {
}

int controllerPackListFilesPollStub(void) {
    return 0;
}

void controllerPackListFiles(s32 channel, controllerPackFileHeader *fileHeaders) {
    OSPfsState pfsState;
    s32 controllerPort;
    s32 err;
    s32 fileIndex;
    s32 baseIndex;
    s32 charIndex;
    u8 *gameName;
    u8 *extName;
    s32 packsOffset;

    controllerPort = channel & 0xFFFF;
    err = osPfsInitPak(&mainStack, &controllerPacks[controllerPort], controllerPort);
    if (err == 0) {
        fileIndex = 0;
        packsOffset = (s32)&controllerPacks[controllerPort] - (s32)controllerPacks;
        gameName = (u8 *)pfsState.game_name;
        extName = (u8 *)pfsState.ext_name;
        baseIndex = controllerPort << 4;
        do {
            if (osPfsFileState((OSPfs *)((s32)controllerPacks + packsOffset), fileIndex, &pfsState) != 0) {
                fileHeaders[baseIndex + fileIndex].gameCode = 0;
                fileHeaders[baseIndex + fileIndex].companyCode = 0;
            } else {
                fileHeaders[baseIndex + fileIndex].fileSize = pfsState.file_size;
                charIndex = 0;
                fileHeaders[baseIndex + fileIndex].gameCode = pfsState.game_code;
                fileHeaders[baseIndex + fileIndex].companyCode = pfsState.company_code;
                do {
                    fileHeaders[baseIndex + fileIndex].gameName[charIndex] = gameName[charIndex];
                    charIndex++;
                } while (charIndex < 16);
                charIndex = 0;
                do {
                    fileHeaders[baseIndex + fileIndex].extName[charIndex] = extName[charIndex];
                    charIndex++;
                } while (charIndex < 4);
            }
            fileIndex++;
        } while (fileIndex < 16);
        err = 0;
    }
    osSendMesg(&D_800A1888_A2488, (OSMesg)err, 1);
}

void controllerPackDeleteFileAsyncStub(void) {
}

int controllerPackDeleteFilePollStub(void) {
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

void controllerPackDeleteFileFromHeaderAsyncStub(void) {
}

int controllerPackDeleteFileFromHeaderPollStub(void) {
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

void controllerPackReadStatusAsyncStub(void) {
}

int controllerPackReadStatusPollStub(void) {
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

void motorUpdateStub(void) {
}

void motorUpdate(void) {
    motorProcessState(&gMotorState);
}

INCLUDE_ASM("asm/nonmatchings/controller_io", motorProcessState);

void startMotorRumble(s32 controllerChannel) {
    s32 channel;
    channel = controllerChannel & 0xFFFF;

    if (((s32)gMotorInitCompleteMask >> channel) & 1) {
        gMotorState.intensity[channel] = 1;
    }
}

void resumeMotorStates(void) {
    u8 *statePtr;
    u8 *end;
    u8 startState = 1;
    s32 i;

    statePtr = (u8 *)gMotorState.state;
    end = statePtr + 4;

    do {
        if (*statePtr == 0) {
            *statePtr = startState;
        }
        statePtr++;
    } while ((s32)statePtr < (s32)end);
}

void initMotorStates(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        gMotorState.state[i] = 0;
        gMotorState.status[i] = 1;
        gMotorState.intensity[i] = 0;
        gMotorState.unk1C[i] = 0;
        gMotorState.duration[i] = 0;
    }
}

void motorInitAsync(s32 channel) {
    s16 temp_v0;
    s32 queueIndex;
    temp_v0 = channel + 0x80;
    queueIndex = D_8008FE8C_90A8C;
    D_800A1C20_A2820[queueIndex].arg = &gMotorState;
    D_800A1C20_A2820[queueIndex].command = temp_v0;
    osSendMesg(&D_800A1820_A2420, (OSMesg *)(&D_800A1C20_A2820[queueIndex]), OS_MESG_BLOCK);
    temp_v0 = ((u16)D_8008FE8C_90A8C) + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
    D_8008FE8F_90A8F = 1;
}

void *pollMotorInitAsync(void) {
    void *result;
    void *status;

    result = NULL;
    status = (void *)-1;
    if (osRecvMesg(&D_800A18A8_A24A8, &result, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        status = result;
    }
    return status;
}

void eepromProbeAsync(void) {
    s16 temp_v0;
    s32 queueIndex;

    D_8008FE8F_90A8F = 1;
    queueIndex = D_8008FE8C_90A8C;
    D_800A1C20_A2820[queueIndex].command = 0xD0;
    osSendMesg(&D_800A1820_A2420, (OSMesg *)&D_800A1C20_A2820[queueIndex], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void *pollEepromProbeAsync(void) {
    void *result;
    void *status;

    result = NULL;
    status = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &result, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        status = result;
    }
    return status;
}

void eepromProbe(void) {
    osSendMesg(&D_800A1888_A2488, (OSMesg *)osEepromProbe(&mainStack), OS_MESG_BLOCK);
}

void eepromReadAsync(s32 slotIndex, void *buffer) {
    s16 temp_v0;
    s32 queueIndex;

    D_8008FE8F_90A8F = 1;
    queueIndex = D_8008FE8C_90A8C;
    D_800A1C20_A2820[queueIndex].arg = buffer;
    D_800A1C20_A2820[queueIndex].command = (slotIndex & 0xFF) + 0xE0;
    osSendMesg(&D_800A1820_A2420, (OSMesg *)&D_800A1C20_A2820[queueIndex], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void *pollEepromReadAsync(void) {
    void *result;
    void *status;

    result = NULL;
    status = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &result, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        status = result;
    }
    return status;
}

void eepromRead(s32 slotIndex, u8 *buffer) {
    u8 blockAddress = ((slotIndex & 0xFF) * 0x10) & 0xF0;
    osSendMesg(&D_800A1888_A2488, (OSMesg *)osEepromLongRead(&mainStack, blockAddress, buffer, 0x58), OS_MESG_BLOCK);
}

void eepromWriteAsync(s32 slotIndex) {
    s16 temp_v0;
    s32 queueIndex;

    D_8008FE8F_90A8F = 1;
    queueIndex = D_8008FE8C_90A8C;
    D_800A1C20_A2820[queueIndex].command = (slotIndex & 0xFF) + 0xF0;
    osSendMesg(&D_800A1820_A2420, &D_800A1C20_A2820[queueIndex], OS_MESG_BLOCK);
    temp_v0 = (u16)D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = temp_v0;
    if (temp_v0 >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void *pollEepromWriteAsync(void) {
    void *result;
    void *status;

    result = NULL;
    status = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &result, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        status = result;
    }
    return status;
}

void eepromWrite(s32 slotIndex) {
    u8 *dataPtr;
    int shiftAmount;
    s32 dataSize;
    s32 checksum;
    s32 byteValue;
    s32 i;
    s32 result;
    dataPtr = EepromSaveData->header_data;
    shiftAmount = 4;
    dataSize = 0x58;
    checksum = 0;

    EepromSaveData->checksum = 0;

    for (i = 0; i < dataSize; i++) {
        byteValue = *dataPtr;
        checksum += byteValue;
        dataPtr++;
    }

    result = (u8)slotIndex;
    result = result << shiftAmount;
    i = result;
    EepromSaveData->checksum = checksum;
    result = osEepromLongWrite(&mainStack, i & 0xF0, (u8 *)EepromSaveData, dataSize);
    osSendMesg(&D_800A1888_A2488, (OSMesg)result, 1);
}

void eepromWriteAllAsync(void *buffer) {
    u32 queueIndexCopy;
    s16 queueIndex;
    s16 *unused;

    queueIndex = D_8008FE8C_90A8C;
    queueIndexCopy = queueIndex;
    D_8008FE8F_90A8F = 1;
    D_800A1C20_A2820[queueIndex].command = 0x140;
    D_800A1C20_A2820[queueIndexCopy].arg = buffer;

    osSendMesg(&D_800A1820_A2420, &D_800A1C20_A2820[queueIndex], OS_MESG_BLOCK);

    queueIndex = D_8008FE8C_90A8C + 1;
    D_8008FE8C_90A8C = queueIndex;

    if (queueIndex >= 0xF) {
        D_8008FE8C_90A8C = 0;
    }
}

void *pollEepromWriteAllAsync(void) {
    void *result;
    void *status;

    result = NULL;
    status = (void *)-1;
    if (osRecvMesg(&D_800A1888_A2488, &result, OS_MESG_NOBLOCK) == 0) {
        D_8008FE8F_90A8F = 0;
        status = result;
    }
    return status;
}

void eepromWriteAll(u8 *buffer) {
    osSendMesg(&D_800A1888_A2488, (OSMesg *)osEepromLongWrite(&mainStack, 0, buffer, 0x200), OS_MESG_BLOCK);
}
