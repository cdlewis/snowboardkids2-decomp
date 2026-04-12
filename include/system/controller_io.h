#ifndef CONTROLLER_IO_H
#define CONTROLLER_IO_H

#include "common.h"

typedef struct {
    s32 fileSize;
    u32 gameCode;
    u16 companyCode;
    u8 gameName[16];
    u8 extName[4];
    u8 padding[6];
} controllerPackFileHeader;

void initControllerSubsystem(void);
void startControllerRead(void);

void processControllerInputs(void);

void controllerPackReadAsyncStub(void);
int controllerPackReadPollStub(void);
void controllerPackWriteAsyncStub(void);
int controllerPackWritePollStub(void);

void resumeMotorStates(void);
void motorUpdate(void);

void *pollEepromWriteAllAsync(void);
void eepromProbeAsync(void);
void *pollEepromProbeAsync(void);

#endif // CONTROLLER_IO_H
