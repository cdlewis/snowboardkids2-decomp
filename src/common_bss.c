#include "PR/libaudio.h"
#include "PRinternal/osint.h"
#include "common.h"
#include "math/geometry.h"
#include "os_message.h"
#include "os_pi.h"
#include "os_thread.h"
#include "os_time.h"
#include "player_fx.h"

#define BSS_ALIGN(n) __attribute__((section(".bss"), aligned(n)))

typedef struct {
    u8 connectedControllerMask;
    u8 bootStack[0x2007];
} BootStack;

void *gCachedPaletteAddr BSS = 0;
Gfx *gDisplayListAllocPtr BSS = 0;
s32 gControllerPackFileCount[4] BSS = { 0 };
u8 gTitleInitialized[2] BSS_ALIGN(1) = { 0 };
s16 gViewportOriginY BSS = 0;
u32 __osBaseCounter BSS = 0;
OSPiHandle DriveRomHandle BSS = { 0 };
Mtx *gLookAtPtr BSS = 0;
OSThread __osThreadSave BSS = { 0 };
u8 storyMapLocationIndex BSS_ALIGN(1) = 0;
u8 gStoryCompleted BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800A8CCA BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800A8CCB BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800A8CCC BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800A8CCD BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800A8CCE BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800A8CCF BSS_ALIGN(1) = 0;
u8 __osContPifRam[0x40] __attribute__((aligned(16), section(".bss"))) = { 0 };
OSMesg gSerialEventQueueMsg BSS = 0;
void *EepromSaveData BSS = 0;
OSPiHandle __Dom2SpeedParam __attribute__((section(".bss"), aligned(8))) = { 0 };
OSMesg gAudioCmdBuffers[2] __attribute__((section(".bss"), aligned(8))) = { 0 };
u8 gAudioManager[0x230] BSS = { 0 };
ALGlobals __libmus_alglobals BSS = { 0 };
static u8 sCommonBssPad_800A9014[4] BSS_ALIGN(1) = { 0 };
BootStack gConnectedControllerMask BSS_ALIGN(1) = { 0 };
u8 bootStackTop[0x24] BSS = { 0 };
s8 gAnalogStickY[4] BSS = { 0 };
s8 gAnalogStickX[3] BSS_ALIGN(1) = { 0 };
u8 gDebugCameraBaseStep BSS_ALIGN(1) = 0;
u32 __osViIntrCount BSS = 0;
s32 gButtonsPressed[4] BSS = { 0 };
s16 gGraphicsMode[2] BSS = { 0 };
s32 gFrameCounter BSS = 0;
void *gActiveViewport BSS = 0;
s32 gParallaxFrameCounter BSS = 0;
s16 gCutsceneSlotIndex[4] BSS = { 0 };
u8 gMotorState[0x24] BSS = { 0 };
u8 __osMaxControllers[4] BSS_ALIGN(1) = { 0 };
OSTime __osCurrentTime BSS = 0;
s32 gRepeatedButtonInputs[4] BSS = { 0 };
OSPiHandle __CartRomHandle BSS = { 0 };
s32 gBufferedFrameCounter BSS = 0;
void *gMemoryAllocatorHead[4] BSS = { 0 };
u8 __osEepPifRam[0x40] __attribute__((aligned(16), section(".bss"))) = { 0 };
OSMesgQueue __osPiAccessQueue __attribute__((section(".bss"), aligned(8))) = { 0 };
s32 gCallbackCounter BSS = 0;
static u8 sCommonBssPad_800AB19C[4] BSS = { 0 };
s32 gPreviousButtonsPressed[4] BSS = { 0 };
OSMesgQueue mainMessageQueue BSS = { 0 };
OSMesg D_800AB1C8_A2538[2] BSS = { 0 };
OSPiHandle __Dom1SpeedParam __attribute__((section(".bss"), aligned(8))) = { 0 };
s16 gParallaxWobbleAngle BSS = 0;
s32 gParallaxReverseDirection BSS = 0;
s32 gControllerPackFreeBlockCount BSS = 0;
s16 gCurrentPoolIndex BSS = 0;
u8 __osContLastCmd BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800AB253 BSS_ALIGN(1) = 0;
OSMesg __osEepromTimerMsg BSS = 0;
u16 gDefaultFontPalette[0x100] BSS = { 0 };
OSTimer __osBaseTimer BSS = { 0 };
s16 gViewportOriginX BSS = 0;
s16 gDebugUnlockEnabled BSS = 0;
u32 __osTimerCounter BSS = 0;
u32 __muscontrol_flag[0xDFC] BSS = { 0 };
u8 mainThreadStack[0x820] BSS = { 0 };
u8 gSerialEventQueue[0x820] BSS = { 0 };
OSTimer __osEepromTimer BSS = { 0 };
void *gCachedTextureAddr BSS = 0;
s16 gTextClipAndOffsetData[6] BSS = { 0 };
s16 gTextureEnabled BSS = 0;
u8 gControllerPollingEnabled BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800AFCE3 BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800AFCE4 BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800AFCE5 BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800AFCE6 BSS_ALIGN(1) = 0;
static u8 sCommonBssPad_800AFCE7 BSS_ALIGN(1) = 0;
u8 __osPfsInodeCache[0x100] __attribute__((section(".bss"), aligned(8))) = { 0 };
__OSEventState __osEventStateTab[OS_NUM_EVENTS] __attribute__((section(".bss"), aligned(8))) = { 0 };
REVERB_MEM gReverbFx BSS = { 0 };
u32 __osFinalrom BSS = 0;
void *gGameSessionContext BSS = 0;
u8 __osPfsPifRam[0x40] __attribute__((aligned(16), section(".bss"))) = { 0 };
u8 gRumblePakReadyMask[8] BSS = { 0 };
OSMesgQueue __osEepromTimerQ BSS = { 0 };
s16 gCutsceneType[4] BSS = { 0 };
OSMesgQueue __osSiAccessQueue __attribute__((section(".bss"), aligned(8))) = { 0 };
s32 gControllerInputs[4] BSS = { 0 };
Vec3i gParallaxOffset BSS = { 0 };
