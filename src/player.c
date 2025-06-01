
#include "common.h"

#define FORNEXT_DEPTH 4

typedef struct {
    u16 wave;
    u16 adsr;
    u8 pan;
    u8 pitch;
} drum_t;

typedef struct {
    u8 padding[0x10];
    /* 0x10 */ u8 *env_table;
    s32 *drum_table;
} song_t;

typedef struct
{
    unsigned char header_name[16];
    unsigned long flags;
    unsigned long wbk_name[3];
    int count;

    unsigned char *basenote;
    float *detune;
    ALWaveTable **wave_list;
} ptr_bank_t;

typedef struct {
    u8 padding000[0x8];
    /* 0x8 */ void *pending;
    /* 0xC */ s32 channel_frame;
    u8 padding001[0x8];
    /* 0x18 */ u32 pitchbend_frame;
    u8 padding002[0x2];
    /* 0x20 */ f32 vib_amount;
    /* 0x24 */ f32 bendrange;
    /* 0x28 */ f32 old_frequency;
    /* 0x2C */ f32 base_note;
    /* 0x30 */ f32 freqoffset;
    /* 0x34 */ u8 *ppitchbend;
    /* 0x38 */ u8 *pvolume;
    u8 padding003[0x8];
    /* 0x44 */ s32 handle;
    /* 0x48 */ s32 priority;
    /* 0x4C */ f32 last_note;
    /* 0x50 */ float port_base;
    u8 padding004[0x4];
    /* 0x58 */ f32 env_attack_calc;
    /* 0x5C */ f32 env_decay_calc;
    /* 0x60 */ f32 env_release_calc;
    /* 0x64 */ s32 env_speed_calc;
    /* 0x68 */ s32 vibrato;
    /* 0x6C */ f32 pitchbend_precalc;
    /* 0x70 */ f32 pitchbend;
    /* 0x74 */ song_t *song_addr;
    /* 0x78 */ ptr_bank_t *sample_bank;
    /* 0x7C */ u8 *pbase;
    /* 0x80 */ drum_t *pdrums;
    /* 0x84 */ u8 *ppitchbendbase;
    /* 0x88 */ u8 *pvolumebase;
    /* 0x8C */ f32 distort;
    u8 padding005[0x4];
    /* 0x94 */ s16 temscale;
    u8 padding006[0x2];
    /* 0x98 */ s16 channel_tempo;
    u8 padding007[0x4];
    /* 0x9E */ s16 cont_vol_repeat_count;
    /* 0xA0 */ u16 cont_pb_repeat_count;
    /* 0xA2 */ u16 fx_addr;
    /* 0xA4 */ s16 channel_tempo_save;
    /* 0xA6 */ u16 count;
    /* 0xA8 */ s16 fixed_length;
    /* 0xAA */ s16 wave;
    u8 padding008[0x1];
    /* 0xAE */ s16 cutoff;
    /* 0xB0 */ s16 endit;
    /* 0xB2 */ u8 vib_delay;
    /* 0xB3 */ u8 ignore;
    /* 0xB4 */ u8 port;
    /* 0xB5 */ u8 transpose;
    /* 0xB6 */ u8 ignore_transpose;
    /* 0xB7 */ s8 velocity;
    /* 0xB8 */ u8 volume;
    /* 0xB9 */ u8 pan;
    u8 padding009[0x1];
    /* 0xBB */ s8 env_speed;
    /* 0xBC */ u8 env_init_vol;
    /* 0xBD */ u8 env_max_vol;
    /* 0xBE */ u8 env_sustain_vol;
    u8 padding010[0x3];
    /* 0xC2 */ u8 env_attack_speed;
    /* 0xC3 */ u8 env_decay_speed;
    /* 0xC4 */ u8 env_release_speed;
    u8 padding011[0x1];
    /* 0xC6 */ u8 reverb;
    u8 padding012[0x3];
    /* 0xCA */ s8 wobble_on_speed;
    /* 0xCB */ s8 wobble_off_speed;
    u8 padding013[0x2];
    /* 0xCE */ u8 velocity_on;
    u8 padding014[0x1];
    /* 0xD0 */ u8 sweep_speed;
    /* 0xD1 */ u8 vib_speed;
    /* 0xD2 */ u8 env_trigger_off;
    /* 0xD3 */ u8 trigger_off;
    /* 0xD4 */ s8 wobble_amount;
    u8 padding015[0x2];
    /* 0xD7 */ u8 for_stack_count;
    /* 0xD8 */ f32 vib_precalc;
    /* 0xDC */ u8 *for_stack[FORNEXT_DEPTH];
    /* 0xEC */ u8 *for_stackvol[FORNEXT_DEPTH];
    /* 0xFC */ u8 *for_stackpb[FORNEXT_DEPTH];
    /* 0x10C */ u16 for_vol_count[FORNEXT_DEPTH];
    /* 0x114 */ u16 for_pb_count[FORNEXT_DEPTH];
    /* 0x11C */ u8 for_count[FORNEXT_DEPTH];
    /* 0x120 */ u8 for_volume[FORNEXT_DEPTH];
    /* 0x124 */ u8 for_pitchbend[FORNEXT_DEPTH];

    u8 padding999[0xC];
} channel_t;

extern ALGlobals __libmus_alglobals;
extern ALVoice *mus_voices;
extern s32 ChangeCustomEffect(u8);

// bss
extern s32 mus_vsyncs_per_second;
extern channel_t *mus_channels;
extern s32 max_channels;
extern s32 mus_songfxchange_flag;

f32 __MusIntPowerOf2(f32 x);

u8 *Fstop(channel_t *cp, u8 *ptr) {
    cp->pvolume = NULL;
    cp->ppitchbend = NULL;
    cp->song_addr = NULL;
    cp->fx_addr = NULL;
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
    return ptr;
}

u8 *Fport(channel_t *cp, u8 *ptr) {
    cp->port = *ptr++;

    if (cp->port) {
        cp->port_base = cp->base_note;
    }

    return ptr;
}

u8 *Fportoff(channel_t *cp, u8 *ptr) {
    cp->port = 0;
    return ptr;
}

u8 *Fdefa(channel_t *cp, u8 *ptr) {
    u8 value;

    // get envelope speed
    value = *ptr++;
    if (value == 0) {
        value = 1;
    }
    cp->env_speed = value;
    cp->env_speed_calc = 1024 / value;

    // get envelope initial volume level
    cp->env_init_vol = *ptr++;

    // get attack speed
    value = *ptr++;

    cp->env_attack_speed = value;

    // get peak volume
    cp->env_max_vol = *ptr++;

    // get attack precalc value
    cp->env_attack_calc = (1.0 / ((float)value)) * ((float)(cp->env_max_vol - cp->env_init_vol));

    // get decay speed
    value = *ptr++;

    cp->env_decay_speed = value;

    // get sustain volume level
    cp->env_sustain_vol = *ptr++;

    // get sustain precalc value
    cp->env_decay_calc = (1.0 / ((float)value)) * ((float)(cp->env_sustain_vol - cp->env_max_vol));

    // get release speed
    value = *ptr++;

    cp->env_release_speed = value;
    cp->env_release_calc = 1.0 / ((float)value);

    return ptr;
}

u8 *Ftempo(channel_t *cp, u8 *ptr) {
    channel_t *sp;
    s32 i;
    s32 temp, temp2;

    temp = (*ptr++) * 256 * 96 / 120 / mus_vsyncs_per_second;
    temp2 = (temp * cp->temscale) >> 7;
    if (cp->fx_addr) {
        cp->channel_tempo = temp;
    } else {
        for (i = 0, sp = mus_channels; i < max_channels; i++, sp++) {
            if (sp->song_addr == cp->song_addr) {
                sp->channel_tempo_save = temp;
                sp->channel_tempo = temp2;
            }
        }
    }
    return ptr;
}

u8 *Fendit(channel_t *cp, u8 *ptr) {
    cp->endit = *ptr++;
    cp->cutoff = 0;
    return ptr;
}

u8 *Fcutoff(channel_t *cp, u8 *ptr) {
    short tmp;

    tmp = (*ptr++) << 8;
    tmp |= *ptr++;

    cp->cutoff = tmp;
    cp->endit = 0;
    return ptr;
}

u8 *Fvibup(channel_t *cp, u8 *ptr) {
    cp->vib_delay = *ptr++;
    cp->vib_speed = *ptr++;
    cp->vib_amount = ((float)*ptr++) / 50.0;
    cp->vib_precalc = (2 * 3.1415926) / (float)cp->vib_speed;
    return ptr;
}

u8 *Fvibdown(channel_t *cp, u8 *ptr) {
    cp->vib_delay = *ptr++;
    cp->vib_speed = *ptr++;
    cp->vib_amount = (-((float)*ptr++)) / 50.0;
    cp->vib_precalc = (2 * 3.1415926) / (float)cp->vib_speed;
    return ptr;
}

u8 *Fviboff(channel_t *cp, u8 *ptr) {
    cp->vib_speed = 0;
    cp->vibrato = 0;
    return ptr;
}

u8 *Flength(channel_t *cp, u8 *ptr) {
    u8 length;
    u16 temp;

    length = *ptr++;
    if ((u8)length < 0x80) {
        cp->fixed_length = length;
    } else {
        temp = (length & 0x7f) << 8;
        cp->fixed_length = temp;
        cp->fixed_length = temp + *ptr++;
    }

    return ptr;
}

u8 *Fignore(channel_t *cp, u8 *ptr) {
    cp->ignore = TRUE;
    return ptr;
}

u8 *Ftrans(channel_t *cp, u8 *ptr) {
    cp->transpose = *ptr++;
    return ptr;
}

u8 *Fignore_trans(channel_t *cp, u8 *ptr) {
    cp->ignore_transpose = 1;
    return ptr;
}

u8 *Fdistort(channel_t *cp, u8 *ptr) {
    int c;
    float f;

    c = (s32)(*ptr++);
    if (c & 0x80) {
        c |= 0xffffff00;
    }
    f = (float)(c) / 100.0;

    cp->freqoffset -= cp->distort;
    cp->freqoffset += f;
    cp->distort = f;
    return ptr;
}

u8 *Fenvelope(channel_t *cp, u8 *ptr) {
    s32 tmp;

    tmp = *ptr++;
    if (tmp & 0x80) {
        tmp &= 0x7f;
        tmp <<= 8;
        tmp |= *ptr++;
    }

    (void)Fdefa(cp, &cp->song_addr->env_table[tmp * 7]);

    return ptr;
}

u8 *Fenvoff(channel_t *cp, u8 *ptr) {
    cp->env_trigger_off = TRUE;
    return ptr;
}

u8 *Fenvon(channel_t *cp, u8 *ptr) {
    cp->env_trigger_off = FALSE;
    return ptr;
}

u8 *Ftroff(channel_t *cp, u8 *ptr) {
    cp->trigger_off = TRUE;
    return ptr;
}

u8 *Ftron(channel_t *cp, u8 *ptr) {
    cp->trigger_off = FALSE;
    return ptr;
}

INCLUDE_ASM("asm/nonmatchings/player", Ffor);

u8 *Fnext(channel_t *cp, u8 *ptr) {
    int index;

    index = cp->for_stack_count - 1;
    if (cp->for_count[index] != 0xff) {
        if (--(cp->for_count[index]) == 0) {
            cp->for_stack_count = index;
            index = -1;
        }
    }

    if (index > -1) {
        ptr = cp->for_stack[index];
        cp->pvolume = cp->for_stackvol[index];
        cp->ppitchbend = cp->for_stackpb[index];
        cp->volume = cp->for_volume[index];
        cp->pitchbend = cp->for_pitchbend[index];
        cp->cont_vol_repeat_count = cp->for_vol_count[index];
        cp->cont_pb_repeat_count = cp->for_pb_count[index];

        // pitchbend_precalc bendrange wrong order?
        cp->bendrange = cp->pitchbend * cp->pitchbend_precalc;
    }

    return ptr;
}

u8 *Fwobble(channel_t *cp, u8 *ptr) {
    cp->wobble_amount = *ptr++;
    cp->wobble_on_speed = *ptr++;
    cp->wobble_off_speed = *ptr++;
    return ptr;
}

u8 *Fwobbleoff(channel_t *cp, u8 *ptr) {
    cp->wobble_on_speed = 0;
    return ptr;
}

u8 *Fvelon(channel_t *cp, u8 *ptr) {
    cp->velocity_on = 1;
    return ptr;
}

u8 *Fveloff(channel_t *cp, u8 *ptr) {
    cp->velocity_on = 0;
    return ptr;
}

INCLUDE_ASM("asm/nonmatchings/player", unknown_libmus_71CE4);

INCLUDE_ASM("asm/nonmatchings/player", unknown_libmus_71CE8);

u8 *Fstereo(channel_t *cp, unsigned char *ptr) {
    return (ptr + 2);
}

u8 *Fdrums(channel_t *cp, u8 *ptr) {
    u32 offset;
    song_t *song;

    song = cp->song_addr;
    offset = song->drum_table[*ptr];
    cp->pdrums = (drum_t *)((u8 *)song + offset);

    return ptr + 1;
}

u8 *Fdrumsoff(channel_t *cp, u8 *ptr) {
    cp->pdrums = NULL;
    return ptr;
}

u8 *Fprint(channel_t *cp, u8 *ptr) {
    ptr++;
    return ptr;
}

u8 *Fgoto(channel_t *cp, u8 *ptr) {
    int off, off1;

    off1 = *ptr++ << 8;
    off1 += *ptr++;

    off = *ptr++ << 8;
    off += *ptr++;
    cp->pvolume = cp->pvolumebase + off;
    cp->cont_vol_repeat_count = 1;

    off = *ptr++ << 8;
    off += *ptr++;
    cp->ppitchbend = cp->ppitchbendbase + off;
    cp->cont_pb_repeat_count = 1;

    return (cp->pbase + off1);
}

u8 *Freverb(channel_t *cp, u8 *ptr) {
    cp->reverb = *ptr++;
    return ptr;
}

u8 *FrandNote(channel_t *cp, u8 *ptr) {
    cp->transpose = __MusIntRandom(*ptr++);
    cp->transpose += *ptr++;
    return ptr;
}

u8 *FrandVolume(channel_t *cp, u8 *ptr) {
    cp->volume = __MusIntRandom(*ptr++);
    cp->volume += *ptr++;
    return ptr;
}

u8 *FrandPan(channel_t *cp, u8 *ptr) {
    cp->pan = __MusIntRandom(*ptr++);
    cp->pan += *ptr++;
    return ptr;
}

u8 *Fvolume(channel_t *cp, u8 *ptr) {
    cp->volume = *ptr++;
    return (ptr);
}

u8 *Fstartfx(channel_t *cp, u8 *ptr) {
    s32 i;
    s32 number;
    channel_t *sp;
    u32 new_handle;

    number = *(ptr++);
    if (number >= 0x80)
        number = ((number & 0x7f) << 8) + (*(ptr++));

    new_handle = __MusIntFindChannelAndStart(number, cp->volume, cp->pan, 0, cp->priority++);
    cp->priority--;

    if (new_handle) {
        for (i = 0, sp = mus_channels; i < max_channels; i++, sp++) {
            if (sp->handle == new_handle) {
                sp->handle = cp->handle;
                sp->sample_bank = cp->sample_bank;
            }
        }
    }
    return ptr;
}

u8 *Fbendrange(channel_t *cp, u8 *ptr) {
    f32 f;
    u8 val;
    f64 d;

    f = cp->pitchbend;
    f *= f;
    val = *ptr++;
    d = (f64)((f32)val);
    d *= (1.0 / 64.0);
    cp->bendrange = f;
    cp->pitchbend_precalc = (f32)d;
    return ptr;
}

u8 *Fsweep(channel_t *cp, u8 *ptr) {
    cp->sweep_speed = *ptr++;
    return ptr;
}

u8 *Fchangefx(channel_t *cp, u8 *ptr) {
    int fxtype;

    fxtype = *ptr++;
    if (mus_songfxchange_flag == TRUE) {
        ChangeCustomEffect(fxtype);
    }

    return (ptr);
}

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

void __MusIntSetPitch(channel_t *cp, s32 x, f32 offset) {
    f32 frequency, temp;

    frequency = cp->base_note;

    if (cp->port != 0) {
        if (cp->count <= cp->port) {
            temp = (frequency - cp->last_note) / (float)(cp->port);
            temp *= (float)cp->count;
            frequency = cp->last_note + temp;
        }
        cp->port_base = frequency;
    }

    frequency += offset + cp->bendrange;  // pitchbend_precalc?

    if (frequency == cp->old_frequency) {
        return;
    }
    cp->old_frequency = frequency;
    frequency = __MusIntPowerOf2(frequency * (1.0 / 12.0));

    if (frequency > 2.0) {
        frequency = 2.0;
        cp->velocity = 0;
    }

    alSynSetPitch(&__libmus_alglobals.drvr, mus_voices + x, frequency);
}

INCLUDE_ASM("asm/nonmatchings/player", func_80073A3C_7463C);

INCLUDE_ASM("asm/nonmatchings/player", func_80073AA4_746A4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073C98_74898);

INCLUDE_ASM("asm/nonmatchings/player", func_80073CB4_748B4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073D6C_7496C);

INCLUDE_ASM("asm/nonmatchings/player", func_80073DC4_749C4);

INCLUDE_ASM("asm/nonmatchings/player", func_80073E20_74A20);

extern f64 D_8009EAB8_9F6B8;

void __MusIntProcessContinuousPitchBend(channel_t *cp) {
    u8 work_pb;

    do {
        cp->pitchbend_frame += 256;
        cp->cont_pb_repeat_count--;
        if (cp->cont_pb_repeat_count == 0) {
            work_pb = *(cp->ppitchbend++);
            if (work_pb > 127) {
                cp->pitchbend = ((float)(work_pb & 0x7f)) - 64.0;
                // order of pitchbend_precalc, bendrange wrong?
                cp->bendrange = cp->pitchbend * cp->pitchbend_precalc;
                work_pb = *(cp->ppitchbend++);
                if (work_pb > 127) {
                    cp->cont_pb_repeat_count = ((int)(work_pb & 0x7f) * 256);
                    cp->cont_pb_repeat_count += (int)*(cp->ppitchbend++) + 2;
                } else
                    cp->cont_pb_repeat_count = (int)work_pb + 2;
            } else {
                cp->pitchbend = ((f32)work_pb) - 64.0;

                // order of pitchbend_precalc, bendrange wrong?
                cp->bendrange = cp->pitchbend * cp->pitchbend_precalc;

                cp->cont_pb_repeat_count = 1;
            }
        }
    } while (cp->pitchbend_frame < cp->channel_frame);
}

f32 __MusIntPowerOf2(f32 x) {
    f32 x2;

    if (x == 0)
        return 1;

    if (x > 0) {
        x2 = x * x;
        return (1 + (x * .693147180559945) + (x2 * .240226506959101) + (x2 * x * 5.55041086648216E-02) + (x2 * x2 * 9.61812910762848E-03) + (x2 * x2 * x * 1.33335581464284E-03) + (x2 * x2 * x2 * 1.54035303933816E-04));
    } else {
        x = -x;
        x2 = x * x;
        return (1 / (1 + (x * .693147180559945) + (x2 * .240226506959101) + (x2 * x * 5.55041086648216E-02) + (x2 * x2 * 9.61812910762848E-03) + (x2 * x2 * x * 1.33335581464284E-03) + (x2 * x2 * x2 * 1.54035303933816E-04)));
    }
}

INCLUDE_ASM("asm/nonmatchings/player", func_8007418C_74D8C);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntRandom);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntInitialiseChannel);

INCLUDE_ASM("asm/nonmatchings/player", func_800744EC_750EC);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntMemSet);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntMemMove);

INCLUDE_ASM("asm/nonmatchings/player", func_800746CC_752CC);

INCLUDE_ASM("asm/nonmatchings/player", func_80074704_75304);

INCLUDE_ASM("asm/nonmatchings/player", __MusIntHandleSetFlag);
