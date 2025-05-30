
#include "common.h"

typedef struct {
    u8 padding[0x8];
    /* 0x8 */ void *pending;
    u8 padding8[0x20];
    /* 0x2C */ f32 base_note;
    u8 padding2[0x4];
    /* 0x34 */ void *ppitchbend;
    /* 0x38 */ s32 pvolume;
    u8 padding5[0x8];
    /* 0x44 */ s32 handle;
    u8 padding9[0x8];
    /* 0x50 */ float port_base;
    u8 padding3[0x20];
    /* 0x74 */ void *song_addr;
    u8 padding4[0x2A];
    /* 0xA2 */ s16 cont_vol_repeat_count;
    u8 padding6[0x6];
    /* 0xAA */ s16 wave;
    u8 padding7[0x8];
    /* 0xB4*/ s8 port;
} channel_t;

u8 *Fstop(channel_t *cp, u8 *ptr) {
    cp->pvolume = NULL;
    cp->ppitchbend = NULL;
    cp->song_addr = NULL;
    cp->cont_vol_repeat_count = NULL;
    cp->handle = 0;
    cp->pending = NULL;

    return NULL;
}

u8 *Fwave(channel_t *cp, u8 *ptr) {
    u16 wave;

    wave = *ptr++;

    if (wave & 0x80) {
        wave &= 0x7f;
        wave <<= 8;
        wave |= *ptr++;
    }

    cp->wave = wave;
    return (ptr);
}

u8 *Fport(channel_t *cp, u8 *ptr) {
    cp->port = *ptr++;

    if (cp->port) {
        cp->port_base = cp->base_note;
    }

    return ptr;
}

INCLUDE_ASM("asm/nonmatchings/player", Fportoff);

INCLUDE_ASM("asm/nonmatchings/player", Fdefa);

INCLUDE_ASM("asm/nonmatchings/player", Ftempo);

INCLUDE_ASM("asm/nonmatchings/player", Fendit);

INCLUDE_ASM("asm/nonmatchings/player", Fcutoff);

INCLUDE_ASM("asm/nonmatchings/player", Fvibup);

INCLUDE_ASM("asm/nonmatchings/player", Fvibdown);

INCLUDE_ASM("asm/nonmatchings/player", Fviboff);

INCLUDE_ASM("asm/nonmatchings/player", Flength);

INCLUDE_ASM("asm/nonmatchings/player", Fignore);

INCLUDE_ASM("asm/nonmatchings/player", Ftrans);

INCLUDE_ASM("asm/nonmatchings/player", Fignore_trans);

INCLUDE_ASM("asm/nonmatchings/player", Fdistort);

INCLUDE_ASM("asm/nonmatchings/player", Fenvelope);

INCLUDE_ASM("asm/nonmatchings/player", Fenvon);

INCLUDE_ASM("asm/nonmatchings/player", Fenvoff);

INCLUDE_ASM("asm/nonmatchings/player", Ftron);

INCLUDE_ASM("asm/nonmatchings/player", Ftroff);

INCLUDE_ASM("asm/nonmatchings/player", Ffor);

INCLUDE_ASM("asm/nonmatchings/player", Fnext);

INCLUDE_ASM("asm/nonmatchings/player", unknown_libmus_71C94);

INCLUDE_ASM("asm/nonmatchings/player", unknown_libmus_71CBC);

INCLUDE_ASM("asm/nonmatchings/player", Fwobble);

INCLUDE_ASM("asm/nonmatchings/player", Fwobbleoff);

INCLUDE_ASM("asm/nonmatchings/player", Fvelon);

INCLUDE_ASM("asm/nonmatchings/player", Fveloff);

INCLUDE_ASM("asm/nonmatchings/player", Fstereo);

INCLUDE_ASM("asm/nonmatchings/player", Fdrums);

INCLUDE_ASM("asm/nonmatchings/player", Fdrumsoff);

INCLUDE_ASM("asm/nonmatchings/player", Fprint);

INCLUDE_ASM("asm/nonmatchings/player", Fgoto);

INCLUDE_ASM("asm/nonmatchings/player", Freverb);

INCLUDE_ASM("asm/nonmatchings/player", FrandNote);

INCLUDE_ASM("asm/nonmatchings/player", FrandVolume);

INCLUDE_ASM("asm/nonmatchings/player", FrandPan);

INCLUDE_ASM("asm/nonmatchings/player", Fvolume);

INCLUDE_ASM("asm/nonmatchings/player", Fstartfx);

INCLUDE_ASM("asm/nonmatchings/player", Fbendrange);

INCLUDE_ASM("asm/nonmatchings/player", Fsweep);

INCLUDE_ASM("asm/nonmatchings/player", Fchangefx);

INCLUDE_ASM("asm/nonmatchings/player", MusInitialize);

INCLUDE_ASM("asm/nonmatchings/player", MusSetMasterVolume);

INCLUDE_ASM("asm/nonmatchings/player", MusStartSongFromMarker);

INCLUDE_ASM("asm/nonmatchings/player", MusStartEffect);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntFindChannelAndStart);

INCLUDE_ASM("asm/nonmatchings/player", func_80072704_73304);

INCLUDE_ASM("asm/nonmatchings/player", func_80072814_73414);

INCLUDE_ASM("asm/nonmatchings/player", func_800728E0_734E0);

INCLUDE_ASM("asm/nonmatchings/player", func_80072960_73560);

INCLUDE_ASM("asm/nonmatchings/player", func_80072A14_73614);

INCLUDE_ASM("asm/nonmatchings/player", func_80072A6C_7366C);

INCLUDE_ASM("asm/nonmatchings/player", func_80072ACC_736CC);

INCLUDE_ASM("asm/nonmatchings/player", func_80072B3C_7373C);

INCLUDE_ASM("asm/nonmatchings/player", func_80072BAC_737AC);

INCLUDE_ASM("asm/nonmatchings/player", func_80072C38_73838);

INCLUDE_ASM("asm/nonmatchings/player", func_80072CC0_738C0);

INCLUDE_ASM("asm/nonmatchings/player", func_80072CDC_738DC);

INCLUDE_ASM("asm/nonmatchings/player", func_80072D20_73920);

INCLUDE_ASM("asm/nonmatchings/player", func_80072D64_73964);

INCLUDE_ASM("asm/nonmatchings/player", func_80072DBC_739BC);

INCLUDE_ASM("asm/nonmatchings/player", MusHandlePause);

INCLUDE_ASM("asm/nonmatchings/player", MusHandleUnPause);

INCLUDE_ASM("asm/nonmatchings/player", MusSetFxType);

INCLUDE_ASM("asm/nonmatchings/player", func_80072E94_73A94);

INCLUDE_ASM("asm/nonmatchings/player", func_80072ED8_73AD8);

INCLUDE_ASM("asm/nonmatchings/player", func_80072F54_73B54);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntFifoProcessCommand);

INCLUDE_ASM("asm/nonmatchings/player", func_80073058_73C58);

INCLUDE_ASM("asm/nonmatchings/player", func_800730EC_73CEC);

INCLUDE_ASM("asm/nonmatchings/player", func_8007335C_73F5C);

INCLUDE_ASM("asm/nonmatchings/player", func_80073738_74338);

INCLUDE_ASM("asm/nonmatchings/player", func_800737C4_743C4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073928_74528);

INCLUDE_ASM("asm/nonmatchings/player", func_80073A3C_7463C);

INCLUDE_ASM("asm/nonmatchings/player", func_80073AA4_746A4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073C98_74898);

INCLUDE_ASM("asm/nonmatchings/player", func_80073CB4_748B4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073D6C_7496C);

INCLUDE_ASM("asm/nonmatchings/player", func_80073DC4_749C4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073E20_74A20);

INCLUDE_ASM("asm/nonmatchings/player", func_80073EE4_74AE4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073FF8_74BF8);

INCLUDE_ASM("asm/nonmatchings/player", func_8007418C_74D8C);

INCLUDE_ASM("asm/nonmatchings/player", func_80074324_74F24);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntInitialiseChannel);

INCLUDE_ASM("asm/nonmatchings/player", func_800744EC_750EC);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntMemSet);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntMemMove);

INCLUDE_ASM("asm/nonmatchings/player", func_800746CC_752CC);

INCLUDE_ASM("asm/nonmatchings/player", func_80074704_75304);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntHandleSetFlag);
