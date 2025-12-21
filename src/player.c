#include "player.h"
#include "audio_manager.h"
#include "common.h"

#define FORNEXT_DEPTH 4
#define MAX_SONGS 4
#define MUSFLAG_EFFECTS 1
#define MUSFLAG_SONGS 2
#define PTRFLAG_REMAPPED (1 << 31)
#define BASEOFFSET 48
#define U8_TO_FLOAT(c) (((c) & 128) ? -(256 - (c)) : (c))

typedef unsigned long musHandle;
typedef void (*LIBMUScb_marker)(musHandle, int);

typedef struct {
    u16 wave;
    u16 adsr;
    u8 pan;
    u8 pitch;
} drum_t;

typedef struct {
    /* 0x00 */ s32 num_channels;
    /* 0x04 */ u8 **channel_data;
    /* 0x08 */ u8 **volume_data;
    /* 0x0C */ u8 **pitchbend_data;
    /* 0x10 */ u8 *env_table;
    /* 0x14 */ s32 *drum_table;
} song_t;

typedef struct {
    u8 header_name[16];
    u32 flags;
    u32 wbk_name[3];
    u32 count;

    u8 *basenote;
    f32 *detune;
    ALWaveTable **wave_list;
} ptr_bank_t;

typedef struct {
    u8 *fxdata;
} fx_t;

typedef struct {
    s32 number_of_components;
    s32 number_of_effects;
    s32 num_waves;
    u32 flags;
    ptr_bank_t *ptr_addr;
    u16 *wave_table;
    fx_t effects[1];
} fx_header_t;

typedef struct {
    u8 command;
    u8 padding1;
    u8 padding2;
    u8 padding3;
    u32 data;
} fifo_t;

typedef struct {
    /* 0x000 */ s32 flags;
    /* 0x004 */ u8 *pdata;
    /* 0x008 */ void *pending;
    /* 0x00C */ u32 channel_frame;
    /* 0x010 */ s32 stopping;
    /* 0x014 */ s32 volume_frame;
    /* 0x018 */ u32 pitchbend_frame;
    /* 0x01C */ s32 stopping_speed;
    /* 0x020 */ f32 vib_amount;
    /* 0x024 */ f32 bendrange;
    /* 0x028 */ f32 old_frequency;
    /* 0x02C */ f32 base_note;
    /* 0x030 */ f32 freqoffset;
    /* 0x034 */ u8 *ppitchbend;
    /* 0x038 */ u8 *pvolume;
    /* 0x03C */ s32 note_end_frame;
    /* 0x040 */ u32 note_start_frame;
    /* 0x044 */ s32 handle;
    /* 0x048 */ s32 priority;
    /* 0x04C */ f32 last_note;
    /* 0x050 */ float port_base;
    /* 0x054 */ u32 release_frame;
    /* 0x058 */ f32 env_attack_calc;
    /* 0x05C */ f32 env_decay_calc;
    /* 0x060 */ f32 env_release_calc;
    /* 0x064 */ s32 env_speed_calc;
    /* 0x068 */ f32 vibrato;
    /* 0x06C */ f32 pitchbend_precalc;
    /* 0x070 */ f32 pitchbend;
    /* 0x074 */ song_t *song_addr;
    /* 0x078 */ fx_header_t *sample_bank;
    /* 0x07C */ u8 *pbase;
    /* 0x080 */ drum_t *pdrums;
    /* 0x084 */ u8 *ppitchbendbase;
    /* 0x088 */ u8 *pvolumebase;
    /* 0x08C */ f32 distort;
    /* 0x090 */ u32 sweep_frame;
    /* 0x094 */ s16 temscale;
    /* 0x096 */ u16 length;
    /* 0x098 */ u16 channel_tempo;
    /* 0x09A */ s16 volscale;
    /* 0x09C */ u16 old_volume;
    /* 0x09E */ u16 cont_vol_repeat_count;
    /* 0x0A0 */ u16 cont_pb_repeat_count;
    /* 0x0A2 */ u16 fx_addr;
    /* 0x0A4 */ s16 channel_tempo_save;
    /* 0x0A6 */ u16 count;
    /* 0x0A8 */ s16 fixed_length;
    /* 0x0AA */ s16 wave;
    /* 0x0AC */ s16 panscale;
    /* 0x0AE */ u16 cutoff;
    /* 0x0B0 */ u16 endit;
    /* 0x0B2 */ u8 vib_delay;
    /* 0x0B3 */ u8 ignore;
    /* 0x0B4 */ u8 port;
    /* 0x0B5 */ u8 transpose;
    /* 0x0B6 */ u8 ignore_transpose;
    /* 0x0B7 */ s8 velocity;
    /* 0x0B8 */ u8 volume;
    /* 0x0B9 */ u8 pan;
    /* 0x0BA */ u8 old_pan;
    /* 0x0BB */ s8 env_speed;
    /* 0x0BC */ u8 env_init_vol;
    /* 0x0BD */ u8 env_max_vol;
    /* 0x0BE */ u8 env_sustain_vol;
    /* 0x0BF */ u8 env_phase;
    /* 0x0C0 */ u8 env_current;
    /* 0x0C1 */ u8 env_count;
    /* 0x0C2 */ u8 env_attack_speed;
    /* 0x0C3 */ u8 env_decay_speed;
    /* 0x0C4 */ u8 env_release_speed;
    /* 0x0C5 */ u8 playing;
    /* 0x0C6 */ u8 reverb;
    u8 padding11;
    /* 0x0C8 */ u8 old_reverb;
    /* 0x0C9 */ u8 release_start_vol;
    /* 0x0CA */ u8 wobble_on_speed;
    /* 0x0CB */ u8 wobble_off_speed;
    /* 0x0CC */ u8 wobble_count;
    /* 0x0CD */ s8 wobble_current;
    /* 0x0CE */ u8 velocity_on;
    /* 0x0CF */ u8 default_velocity;
    /* 0x0D0 */ u8 sweep_speed;
    /* 0x0D1 */ u8 vib_speed;
    /* 0x0D2 */ u8 env_trigger_off;
    /* 0x0D3 */ u8 trigger_off;
    /* 0x0D4 */ s8 wobble_amount;
    /* 0x0D5 */ u8 sweep_timer;
    /* 0x0D6 */ u8 sweep_dir;
    /* 0x0D7 */ u8 for_stack_count;
    /* 0x0D8 */ f32 vib_precalc;
    /* 0x0DC */ u8 *for_stack[FORNEXT_DEPTH];
    /* 0x0EC */ u8 *for_stackvol[FORNEXT_DEPTH];
    /* 0x0FC */ u8 *for_stackpb[FORNEXT_DEPTH];
    /* 0x10C */ u16 for_vol_count[FORNEXT_DEPTH];
    /* 0x114 */ u16 for_pb_count[FORNEXT_DEPTH];
    /* 0x11C */ u8 for_count[FORNEXT_DEPTH];
    /* 0x120 */ u8 for_volume[FORNEXT_DEPTH];
    /* 0x124 */ u8 for_pitchbend[FORNEXT_DEPTH];

    u8 padding999[0xC];
} channel_t;

extern ALGlobals __libmus_alglobals;
extern ALVoice *mus_voices;
extern s32 ChangeCustomEffect(s32);
extern ALMicroTime mus_next_frame_time;
extern fx_header_t *libmus_fxheader_current;
extern fx_header_t *libmus_fxheader_single;

// bss
extern s32 mus_vsyncs_per_second;
extern channel_t *mus_channels;
extern s32 max_channels;
extern s32 mus_songfxchange_flag;

extern LIBMUScb_marker marker_callback;
extern s32 mus_last_fxtype;
extern s32 mus_current_handle;
extern s32 mus_random_seed;
extern ALPlayer plr_player;
extern u16 mus_master_volume_effects;
extern u16 mus_master_volume_songs;

u32 __muscontrol_flag;
fx_t *D_800A64F4_A70F4;
s32 *D_800A64F8_A70F8;
ALHeap audio_heap;
s32 D_800A6520_A7120;
s32 D_800A6524_A7124;
s32 D_800A6528_A7128;
void *D_800A652C_A712C;

void func_80072F54_73B54(void);
void func_80073738_74338(channel_t *cp, int x);
void func_8007335C_73F5C(channel_t *cp, int x);
void func_80073E20_74A20(channel_t *cp);
void __MusIntProcessContinuousPitchBend(channel_t *cp);
u8 *Fstop(channel_t *cp, u8 *ptr);
void func_80073AA4_746A4(channel_t *cp);
void func_80073CB4_748B4(channel_t *cp);
f32 func_80073DC4_749C4(channel_t *cp);
f32 func_80073D6C_7496C(channel_t *cp);
void __MusIntSetPitch(channel_t *cp, s32 x, f32 offset);
void func_800737C4_743C4(channel_t *cp, s32 x);
f32 __MusIntPowerOf2(f32 x);
void MusSetMasterVolume(u32 flags, u32 volume);
void __MusIntInitialiseChannel(channel_t *cp);
u32 __MusIntStartEffect(channel_t *cp, s32 number, s32 volume, s32 pan, s32 priority);
ALMicroTime __MusIntMain(void *node);
void __MusIntRemapPtrs(void *addr, void *offset, s32 count);
void MusHandleUnPause(musHandle);
void __MusIntFifoProcessCommand(fifo_t *command);
void __MusIntHandleSetFlag(u32 handle, u32 clear, u32 set);
s32 func_800744EC_750EC(song_t *, s32);
void MusPtrBankInitialize(void *, u8 *);
void __MusIntFifoOpen(s32);
void __MusIntMemSet(void *, unsigned char, int);
void __MusIntMemMove(u8 *, u8 *, s32);
s32 func_80073058_73C58(u8 *);
u32 func_800725F4_731F4(s32, s32, s32, s32, s32);
s32 __MusIntRandom(s32);

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
    if (length < 0x80) {
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

u8 *Ffor(channel_t *cp, u8 *ptr) {
    s32 index;

    index = cp->for_stack_count;
    cp->for_count[index] = *ptr++;
    cp->for_stack[index] = ptr;
    cp->for_stackvol[index] = cp->pvolume;
    cp->for_stackpb[index] = cp->ppitchbend;
    cp->for_volume[index] = cp->volume;
    cp->for_pitchbend[index] = cp->pitchbend;
    cp->for_vol_count[index] = cp->cont_vol_repeat_count;
    cp->for_pb_count[index] = cp->cont_pb_repeat_count;
    cp->for_stack_count++;

    return ptr;
}

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

u8 *Fvelocity(channel_t *cp, u8 *ptr) {
    cp->default_velocity = *ptr++;
    cp->velocity_on = 0;
    return ptr;
}

u8 *Fpan(channel_t *cp, u8 *ptr) {
    cp->pan = (*ptr++) / 2;
    return ptr;
}

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

    new_handle = func_800725F4_731F4(number, cp->volume, cp->pan, 0, cp->priority++);
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
    s32 fxtype;

    fxtype = *ptr++;
    if (mus_songfxchange_flag == TRUE) {
        ChangeCustomEffect(fxtype);
    }

    return ptr;
}

s32 MusInitialize(musConfig *config) {
    ALVoiceConfig vc;
    s32 i;
    ALSynConfig audioConfig;
    AudioParams audioParams;

    __muscontrol_flag = config->control_flag;

    max_channels = config->channels;

    D_800A64F4_A70F4 = config->sched;
    D_800A64F8_A70F8 = (s32 *)config->default_fxbank;

    if (osTvType == OS_TV_PAL) {
        mus_vsyncs_per_second = 50;
    } else {
        mus_vsyncs_per_second = 60;
    }
    mus_next_frame_time = 1000000 / mus_vsyncs_per_second;

    __MusIntMemSet(config->heap, 0, config->heap_length);

    alHeapInit(&audio_heap, config->heap, config->heap_length);

    mus_voices = (ALVoice *)alHeapDBAlloc(0, 0, &audio_heap, 1, max_channels * (sizeof(ALVoice)));
    mus_channels = (channel_t *)alHeapDBAlloc(0, 0, &audio_heap, 1, max_channels * (sizeof(channel_t)));
    __MusIntFifoOpen(config->fifo_length);
    __MusIntMemSet((u8 *)mus_voices, 0, max_channels * (sizeof(ALVoice)));
    __MusIntMemSet((u8 *)mus_channels, 0, max_channels * (sizeof(channel_t)));

    if (config->ptr) {
        MusPtrBankInitialize(config->ptr, config->wbk);
        libmus_fxheader_single = (fx_header_t *)config->ptr;
        libmus_fxheader_current = libmus_fxheader_single;
    } else {
        libmus_fxheader_current = NULL;
    }

    audioConfig.maxVVoices = max_channels;
    audioConfig.maxPVoices = max_channels;
    audioConfig.maxUpdates = config->syn_output_rate;
    audioConfig.fxType = 2;
    audioConfig.dmaproc = 0;
    audioConfig.outputRate = 0;
    audioConfig.heap = &audio_heap;
    audioParams.syn_output_rate = config->syn_rsp_cmds;
    audioParams.sample_rate = config->syn_num_dma_bufs;
    audioParams.syn_rsp_cmds = config->syn_retraceCount;

    mus_last_fxtype = AL_FX_BIGROOM;
    initAudioManager(
        &audioConfig,
        config->thread_priority,
        &audioParams,
        config->syn_dma_buf_size,
        (s32)config->diskrom_handle,
        mus_vsyncs_per_second
    );

    MusSetMasterVolume(3, 0x7fff);

    mus_current_handle = 1;
    mus_random_seed = 0x12345678;

    plr_player.next = NULL;
    plr_player.handler = __MusIntMain;
    plr_player.clientData = &plr_player;
    alSynAddPlayer(&__libmus_alglobals.drvr, &plr_player);

    for (i = 0; i < max_channels; i++) {
        mus_channels[i].playing = 0;
        __MusIntInitialiseChannel(&mus_channels[i]);
        vc.unityPitch = 0;
        vc.priority = config->thread_priority;
        vc.fxBus = 0;
        alSynAllocVoice(&__libmus_alglobals.drvr, &mus_voices[i], &vc);
    }

    return audio_heap.cur - audio_heap.base;
}

void MusSetMasterVolume(u32 flags, u32 volume) {
    if (flags & MUSFLAG_EFFECTS)
        mus_master_volume_effects = volume;
    if (flags & MUSFLAG_SONGS)
        mus_master_volume_songs = volume;
}

musHandle __MusIntStartSong(void *addr, int marker) {
    song_t *song;
    s32 num_channels;
    s32 i;
    s32 channel_index;
    channel_t *cp;
    musHandle handle;

    song = (song_t *)addr;
    num_channels = song->num_channels;

    if (song->channel_data < (u8 **)0x400) {
        __MusIntRemapPtrs(&song->channel_data, song, 5);
        __MusIntRemapPtrs(song->channel_data, song, num_channels);
        __MusIntRemapPtrs(song->volume_data, song, num_channels);
        __MusIntRemapPtrs(song->pitchbend_data, song, num_channels);
    }

    handle = mus_current_handle++;

    for (i = 0; i < num_channels; i++) {
        if (song->channel_data[i] != 0) {
            channel_index = func_800744EC_750EC(song, i);
            cp = &mus_channels[channel_index];
            __MusIntInitialiseChannel(cp);
            cp->song_addr = song;
            cp->flags |= 1;
            cp->pvolume = cp->pvolumebase = song->volume_data[i];
            cp->ppitchbend = cp->ppitchbendbase = song->pitchbend_data[i];
            cp->pdata = cp->pbase = song->channel_data[i];
            cp->handle = handle;
        }
    }

    MusHandleUnPause(handle);
    return handle;
}

u32 __MusIntFindChannelAndStart(s32 number) {
    s32 i;
    s32 priority;
    s32 current_priority;
    channel_t *cp;
    channel_t *current_cp;

    priority = D_800A64F8_A70F8[number];
    current_priority = priority + 1;

    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if (cp->pdata == 0) {
            return __MusIntStartEffect(cp, number, 0x80, 0x80, priority);
        }

        if (cp->fx_addr && (cp->priority < current_priority)) {
            current_priority = cp->priority;
            current_cp = cp;
        }
    }

    if (current_priority < priority) {
        return __MusIntStartEffect(current_cp, number, 0x80, 0x80, priority);
    }

    return 0;
}

u32 func_800725F4_731F4(s32 number, s32 volume, s32 pan, s32 arg3, s32 priority) {
    s32 i;
    s32 current_priority;
    channel_t *cp;
    channel_t *current_cp;

    if (priority == -1) {
        priority = D_800A64F8_A70F8[number];
    }

    if (arg3 != 0) {
        for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
            if (cp->fx_addr == number) {
                return __MusIntStartEffect(cp, number, volume, pan, priority);
            }
        }
    }

    current_priority = priority + 1;

    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if (cp->pdata == 0) {
            return __MusIntStartEffect(cp, number, volume, pan, priority);
        }

        if (cp->fx_addr && (cp->priority < current_priority)) {
            current_priority = cp->priority;
            current_cp = cp;
        }
    }

    if (current_priority < priority) {
        return __MusIntStartEffect(current_cp, number, volume, pan, priority);
    }

    return 0;
}

u32 func_80072704_73304(s32 number, s32 volume, s32 pan, s32 handle, s32 priority) {
    s32 i;
    s32 current_priority;
    channel_t *cp;
    channel_t *current_cp;

    if (priority == -1) {
        priority = D_800A64F8_A70F8[number];
    }

    if (handle != 0) {
        for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
            if (cp->handle == handle) {
                return __MusIntStartEffect(cp, number, volume, pan, priority);
            }
        }
    }

    current_priority = priority + 1;

    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if (cp->pdata == 0) {
            return __MusIntStartEffect(cp, number, volume, pan, priority);
        }

        if (cp->fx_addr && (cp->priority < current_priority)) {
            current_priority = cp->priority;
            current_cp = cp;
        }
    }

    if (current_priority < priority) {
        return __MusIntStartEffect(current_cp, number, volume, pan, priority);
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/player", func_80072814_73414);

s32 func_800728E0_734E0(u32 flags) {
    s32 i;
    s32 count;
    channel_t *cp;

    for (i = 0, cp = mus_channels, count = 0; i < max_channels; i++, cp++) {
        if (cp->pdata) {
            if ((cp->fx_addr && (flags & MUSFLAG_EFFECTS)) || (!cp->fx_addr && (flags & MUSFLAG_SONGS))) {
                count++;
            }
        }
    }
    return count;
}

s32 func_80072960_73560(u32 handle, s32 stopSpeed) {
    channel_t *channel;
    s32 count;
    s32 i;
    s32 speed;

    if (handle == 0) {
        return 0;
    }

    speed = stopSpeed;
    if (stopSpeed == 0) {
        speed = 1;
    }

    count = 0;
    for (i = 0, channel = mus_channels; i < max_channels; i++, channel++) {
        if (channel->handle == handle && channel->stopping == -1) {
            if (channel->flags & 1) {
                channel->stopping_speed = 1;
                channel->stopping = 0;
                channel->flags &= ~1;
            } else {
                channel->stopping_speed = speed;
                channel->stopping = stopSpeed;
            }
            count++;
        }
    }

    return count;
}

void *func_80072A14_73614(musHandle arg0) {
    s32 i;
    channel_t *channel;
    s32 count;

    if (!arg0) {
        return 0;
    }

    i = 0;
    channel = mus_channels;
    count = 0;

    while (i < max_channels) {
        i++;
        if (channel->handle == arg0) {
            count++;
        }
        channel++;
    }

    return (void *)count;
}

s32 func_80072A6C_7366C(void *handle, s32 volume) {
    s32 i;
    s32 count;
    channel_t *cp;

    if (handle == NULL) {
        return 0;
    }

    count = 0;
    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if ((void *)cp->handle == handle) {
            cp->volscale = volume;
            count++;
        }
    }
    return count;
}

s32 func_80072ACC_736CC(void *handle, s32 pan) {
    s32 i;
    s32 count;
    channel_t *cp;

    if (handle == NULL) {
        return 0;
    }

    count = 0;
    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if ((void *)cp->handle == handle) {
            count++;
            cp->panscale = pan;
            cp->old_pan = 0xFF;
        }
    }
    return count;
}

s32 func_80072B3C_7373C(void *handle, f32 offset) {
    s32 i;
    s32 count;
    channel_t *cp;

    if (handle == NULL) {
        return 0;
    }

    count = 0;
    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if ((void *)cp->handle == handle) {
            cp->freqoffset = offset + cp->distort;
            count++;
        }
    }
    return count;
}

s32 func_80072BAC_737AC(void *arg0, s32 arg1) {
    s32 count;
    s32 i;
    channel_t *cp;
    u16 temp;

    if (arg0 == NULL) {
        return 0;
    }

    if (arg1 <= 0) {
        arg1 = 1;
    } else if (arg1 > 0x100) {
        arg1 = 0x100;
    }

    count = 0;
    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if ((void *)cp->handle == arg0) {
            count++;
            cp->temscale = arg1;
            temp = cp->channel_tempo_save;
            cp->channel_tempo = (temp * arg1) >> 7;
        }
    }

    return count;
}

s32 func_80072C38_73838(void *arg0, s32 arg1) {
    s32 count;
    s32 i;
    channel_t *cp;

    if (arg0 == NULL) {
        return 0;
    }

    if (arg1 < 0) {
        arg1 = 0;
    } else if (arg1 >= 0x80) {
        arg1 = 0x7F;
    }

    count = 0;
    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if ((void *)cp->handle == arg0) {
            count++;
            cp->padding11 = arg1;
            cp->old_reverb = 0xFF;
        }
    }

    return count;
}

void func_80072CC0_738C0(void *pbank, void *wbank) {
    MusPtrBankInitialize(pbank, wbank);
}

musHandle func_80072CDC_738DC(fx_header_t *arg0, void *arg1) {
    musHandle result;

    if (arg0 != NULL) {
        s32 temp = (s32)arg0->ptr_addr;
        if (temp < 0) {
            libmus_fxheader_single = arg0;
        }
    }

    result = ((s32(*)(void *))__MusIntStartSong)(arg1);
    libmus_fxheader_single = libmus_fxheader_current;

    return result;
}

musHandle func_80072D20_73920(fx_header_t *arg0, s32 arg1) {
    musHandle result;

    if (arg0 != NULL) {
        s32 temp = (s32)arg0->ptr_addr;
        if (temp < 0) {
            libmus_fxheader_single = arg0;
        }
    }

    result = __MusIntFindChannelAndStart(arg1);
    libmus_fxheader_single = libmus_fxheader_current;

    return result;
}

musHandle func_80072D64_73964(fx_header_t *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    musHandle result;

    if (arg0 != NULL) {
        if ((s32)arg0->ptr_addr < 0) {
            libmus_fxheader_single = arg0;
        }
    }

    result = func_800725F4_731F4(arg1, arg2, arg3, arg4, arg5);
    libmus_fxheader_single = libmus_fxheader_current;

    return result;
}

fx_header_t *func_80072DBC_739BC(s32 handle) {
    s32 i;
    channel_t *sp;

    if (handle == 0) {
        return NULL;
    }

    for (i = 0, sp = mus_channels; i < max_channels; i++, sp++) {
        if (sp->handle == handle) {
            return sp->sample_bank;
        }
    }

    return NULL;
}

void MusHandlePause(s32 arg0) {
    struct {
        u8 byte_at_10;
        u8 pad[3];
        s32 word_at_14;
    } locals;
    volatile s32 *prevent_opt;

    prevent_opt = &locals.word_at_14;
    *prevent_opt = arg0;
    locals.byte_at_10 = 0;
    func_80073058_73C58(&locals.byte_at_10);
}

void MusHandleUnPause(musHandle arg0) {
    struct {
        u8 byte_at_10;
        u8 pad[3];
        musHandle word_at_14;
    } locals;
    volatile musHandle *prevent_opt;

    prevent_opt = &locals.word_at_14;
    *prevent_opt = arg0;
    locals.byte_at_10 = 1;
    func_80073058_73C58(&locals.byte_at_10);
}

int MusSetFxType(int fxtype) {
    struct {
        u8 byte_at_10;
        u8 pad[3];
        s32 word_at_14;
    } locals;
    volatile s32 *prevent_opt;

    prevent_opt = &locals.word_at_14;
    *prevent_opt = fxtype;
    mus_last_fxtype = fxtype;
    locals.byte_at_10 = 2;

    return func_80073058_73C58(&locals.byte_at_10);
}

void func_80072E94_73A94(int onoff) {
    int changed = 1;

    if (onoff == 0) {
        changed = MusSetFxType(mus_last_fxtype);
    }

    if (changed) {
        mus_songfxchange_flag = onoff;
    }
}

void __MusIntFifoOpen(s32 commands) {
    void *temp;

    if (commands < 0x40) {
        commands = 0x40;
    } else if (commands >= 0x401) {
        commands = 0x400;
    }

    temp = alHeapDBAlloc(0, 0, &audio_heap, 1, commands * 8);
    D_800A652C_A712C = temp;
    D_800A6528_A7128 = commands;
    D_800A6524_A7124 = 0;
    D_800A6520_A7120 = 0;
}

void func_80072F54_73B54(void) {
    while (D_800A6520_A7120 != D_800A6524_A7124) {
        __MusIntFifoProcessCommand(&((fifo_t *)D_800A652C_A712C)[D_800A6520_A7120]);
        D_800A6520_A7120++;
        if (D_800A6520_A7120 == D_800A6528_A7128) {
            D_800A6520_A7120 = 0;
        }
    }
}

void __MusIntFifoProcessCommand(fifo_t *command) {
    switch (command->command) {
        case 0:
            __MusIntHandleSetFlag(command->data, -2, 1);
            break;
        case 1:
            __MusIntHandleSetFlag(command->data, -2, 0);
            break;
        case 2:
            ChangeCustomEffect(command->data);
            break;
    }
}

s32 func_80073058_73C58(u8 *arg0) {
    s32 next_idx;
    s32 current_idx;

    current_idx = D_800A6524_A7124;
    next_idx = (current_idx + 1) % D_800A6528_A7128;

    if (next_idx == D_800A6520_A7120) {
        return 0;
    }

    __MusIntMemMove(arg0, (u8 *)D_800A652C_A712C + (current_idx * 8), 8);
    D_800A6524_A7124 = next_idx;

    return 1;
}

ALMicroTime __MusIntMain(void *node) {
    channel_t *cp;
    f32 total;
    s32 x;

    func_80072F54_73B54();

    for (x = 0, cp = mus_channels; x < max_channels; x++, cp++) {
        if (cp->pdata == NULL || (cp->flags & 1)) {
            continue;
        }

        if (cp->pending != NULL) {
            func_80073738_74338(cp, x);
        }

        cp->channel_frame += cp->channel_tempo;

        if (cp->length != 0x7FFF) {
            while ((u32)cp->note_end_frame < cp->channel_frame) {
                if (cp->pdata == NULL) {
                    break;
                }
                func_8007335C_73F5C(cp, x);
            }

            if (cp->pdata == NULL) {
                continue;
            }
        }

        if (cp->pvolume != NULL && (u32)cp->volume_frame < cp->channel_frame) {
            func_80073E20_74A20(cp);
        }

        if (cp->ppitchbend != NULL && cp->pitchbend_frame < cp->channel_frame) {
            __MusIntProcessContinuousPitchBend(cp);
        }

        if (cp->stopping != -1) {
            cp->stopping--;
            if (cp->stopping == -1) {
                cp->pdata = Fstop(cp, NULL);
                if (cp->playing != 0) {
                    cp->playing = 0;
                    alSynStopVoice(&__libmus_alglobals.drvr, mus_voices + x);
                }
            }
        }

        if (cp->playing != 0) {
            if (cp->env_phase != 0) {
                func_80073AA4_746A4(cp);
            }

            if (cp->sweep_speed != 0 && cp->sweep_frame < cp->channel_frame) {
                func_80073CB4_748B4(cp);
            }

            total = cp->freqoffset;

            if (cp->vib_speed != 0) {
                total += func_80073DC4_749C4(cp);
            }

            if (cp->wobble_on_speed != 0) {
                total += func_80073D6C_7496C(cp);
            }

            if (cp->pending == NULL) {
                __MusIntSetPitch(cp, x, total);
                func_800737C4_743C4(cp, x);
            }
        }

        cp->count = (cp->channel_frame - cp->note_start_frame) >> 8;
    }

    return mus_next_frame_time;
}

INCLUDE_ASM("asm/nonmatchings/player", func_8007335C_73F5C);

void func_80073738_74338(channel_t *cp, int x) {
    if (cp->playing) {
        alSynStopVoice(&__libmus_alglobals.drvr, mus_voices + x);
    }
    cp->playing = 1;
    alSynStartVoice(&__libmus_alglobals.drvr, mus_voices + x, cp->pending);
    cp->pending = NULL;
}

void func_800737C4_743C4(channel_t *cp, s32 x) {
    u32 volume;
    s32 stopping;
    u8 pan;

    volume = ((u32)(cp->volume * cp->env_current * (u8)cp->velocity * cp->volscale)) >> 13;
    if (0x7FFFU < volume) {
        volume = 0x7FFF;
    }

    if (cp->fx_addr == 0) {
        volume = (volume * mus_master_volume_songs);
    } else {
        volume = (volume * mus_master_volume_effects);
    }

    stopping = cp->stopping;
    volume = volume >> 15;

    if (stopping != -1) {
        volume = (volume * stopping) / cp->stopping_speed;
    }

    if (volume != cp->old_volume) {
        cp->old_volume = volume;
        alSynSetVol(&__libmus_alglobals.drvr, mus_voices + x, (s16)volume, mus_next_frame_time);
    }

    pan = cp->pan;
    if (pan != cp->old_pan) {
        pan = ((pan * cp->panscale) >> 7) & 0x7F;
        volume = pan;
        cp->old_pan = volume;
        alSynSetPan(&__libmus_alglobals.drvr, mus_voices + x, pan);
    }
}

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

    frequency += offset + cp->bendrange; // pitchbend_precalc?

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

void func_80073A3C_7463C(channel_t *arg0) {
    channel_t *chan = arg0;
    u16 temp;

    if (chan->length != 0x7FFF) {
        temp = chan->cutoff;
        if (temp != 0) {
            chan->release_frame = chan->note_start_frame + (temp << 8);
        } else {
            chan->release_frame = chan->note_end_frame - (chan->endit << 8);
        }
    } else {
        chan->release_frame = 0x7FFFFFFF;
    }

    chan->env_phase = 1;
    chan->env_current = chan->env_init_vol;
    chan->env_count = chan->env_speed;
}

void func_80073AA4_746A4(channel_t *cp) {
    s32 progress;
    u8 phase;

    if (cp->channel_frame >= cp->release_frame && cp->env_phase < 4) {
        cp->env_phase = 4;
        cp->env_count = 1;
        cp->release_start_vol = cp->env_current;
    }

    cp->env_count--;
    if (cp->env_count != 0) {
        return;
    }

    phase = cp->env_phase;
    cp->env_count = cp->env_speed;

    switch (phase) {
        case 1:
            progress = (u32)((cp->channel_frame - cp->note_start_frame) >> 8) * cp->env_speed_calc >> 10;
            if (progress < cp->env_attack_speed) {
                cp->env_current = cp->env_init_vol + (s32)(cp->env_attack_calc * (f32)progress);
            } else {
                cp->env_phase = phase + 1;
                cp->env_current = cp->env_max_vol;
            }
            break;
        case 2:
            progress =
                (u32)(((cp->channel_frame - cp->note_start_frame) >> 8) - cp->env_attack_speed) * cp->env_speed_calc >>
                10;
            if (progress < cp->env_decay_speed) {
                cp->env_current = cp->env_max_vol + (s32)(cp->env_decay_calc * (f32)progress);
            } else {
                cp->env_phase = phase + 1;
                cp->env_current = cp->env_sustain_vol;
            }
            break;
        case 3:
            break;
        case 4:
            progress = (u32)((cp->channel_frame - cp->release_frame) >> 8) * cp->env_speed_calc >> 10;
            if (progress < cp->env_release_speed) {
                cp->env_current =
                    cp->release_start_vol - (s32)(cp->env_release_calc * (f32)progress * (f32)cp->release_start_vol);
            } else {
                cp->env_phase = phase + 1;
                cp->env_current = 0;
            }
            break;
    }
}

void func_80073C98_74898(channel_t *cp) {
    s32 temp = cp->note_start_frame;
    u8 val = cp->pan;

    cp->sweep_timer = 0;
    cp->sweep_frame = temp;
    cp->sweep_dir = val & 0x40;
}

void func_80073CB4_748B4(channel_t *cp) {
    u32 calc;

    do {
        calc = cp->sweep_timer + cp->sweep_speed;
        cp->sweep_frame += 0x100;

        if (calc < 0x40) {
            cp->sweep_timer = calc;
        } else {
            cp->sweep_timer = calc & 0x3F;
            calc >>= 6;

            if (cp->sweep_dir == 0) {
                u8 pan = cp->pan + calc;

                cp->pan = pan;
                if (pan >= 0x80) {
                    cp->pan = 0x7F;
                    cp->sweep_dir = 1;
                }
            } else {
                u8 pan = cp->pan - calc;

                cp->pan = pan;
                if ((pan >= 0x80) || (pan == 0)) {
                    cp->pan = 0;
                    cp->sweep_dir = 0;
                }
            }
        }
    } while (cp->sweep_frame < cp->channel_frame);
}

f32 func_80073D6C_7496C(channel_t *cp) {
    cp->wobble_count--;
    if (cp->wobble_count == 0) {
        if (cp->wobble_current == 0) {
            cp->wobble_current = cp->wobble_amount;
            cp->wobble_count = cp->wobble_on_speed;
        } else {
            cp->wobble_count = cp->wobble_off_speed;
            cp->wobble_current = 0;
        }
    }
    return (f32)cp->wobble_current;
}

f32 func_80073DC4_749C4(channel_t *cp) {
    s32 temp;

    temp = cp->count - cp->vib_delay;

    if (temp > 0) {
        cp->vibrato = sinf((f32)temp * cp->vib_precalc) * cp->vib_amount;
    }

    return cp->vibrato;
}

void func_80073E20_74A20(channel_t *cp) {
    u8 work_vol;
    u16 temp;

    do {
        cp->volume_frame += 256;
        if (!((temp = cp->cont_vol_repeat_count - 1, cp->cont_vol_repeat_count = temp) & 0xFFFF)) {
            work_vol = *(cp->pvolume++);
            if (work_vol > 127) {
                cp->volume = work_vol & 0x7f;
                work_vol = *(cp->pvolume++);
                if (work_vol > 127) {
                    cp->cont_vol_repeat_count = ((int)(work_vol & 0x7f) * 256);
                    cp->cont_vol_repeat_count += (int)*(cp->pvolume++) + 2;
                } else {
                    cp->cont_vol_repeat_count = (int)work_vol + 2;
                }
            } else {
                cp->volume = work_vol;
                cp->cont_vol_repeat_count = 1;
            }
        }
    } while ((u32)cp->volume_frame < cp->channel_frame);
}

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
        return (
            1 + (x * .693147180559945) + (x2 * .240226506959101) + (x2 * x * 5.55041086648216E-02) +
            (x2 * x2 * 9.61812910762848E-03) + (x2 * x2 * x * 1.33335581464284E-03) +
            (x2 * x2 * x2 * 1.54035303933816E-04)
        );
    } else {
        x = -x;
        x2 = x * x;
        return (
            1 / (1 + (x * .693147180559945) + (x2 * .240226506959101) + (x2 * x * 5.55041086648216E-02) +
                 (x2 * x2 * 9.61812910762848E-03) + (x2 * x2 * x * 1.33335581464284E-03) +
                 (x2 * x2 * x2 * 1.54035303933816E-04))
        );
    }
}

void MusPtrBankInitialize(void *pptr, u8 *wptr) {
    ptr_bank_t *ptrfile_addr;
    s32 i;
    u8 *chardetune;
    u8 charwork;
    f32 *floatdetune;
    f32 floatwork;
    u32 base;

    ptrfile_addr = (ptr_bank_t *)pptr;

    if (ptrfile_addr->flags & PTRFLAG_REMAPPED) {
        return;
    }

    ptrfile_addr->flags |= PTRFLAG_REMAPPED;

    __MusIntRemapPtrs(&ptrfile_addr->basenote, ptrfile_addr, 3);
    __MusIntRemapPtrs(ptrfile_addr->wave_list, ptrfile_addr, ptrfile_addr->count);

    for (i = 0; i < (s32)ptrfile_addr->count; i++) {
        floatdetune = &ptrfile_addr->detune[i];
        chardetune = (u8 *)floatdetune;
        charwork = *chardetune;

        floatwork = U8_TO_FLOAT(charwork);
        *floatdetune = floatwork / 100.0;

        charwork = ptrfile_addr->basenote[i] - BASEOFFSET;
        floatwork = U8_TO_FLOAT(charwork);
        *floatdetune += floatwork;

        if (!ptrfile_addr->wave_list[i]->flags) {
            base = (u32)ptrfile_addr->wave_list[i]->base;
            if ((base & 0xFF000000) != 0xFF000000) {
                base += (u32)wptr;
                ptrfile_addr->wave_list[i]->base = (u8 *)base;
            }
            ptrfile_addr->wave_list[i]->flags = 1;

            if (ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop) {
                ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop =
                    (ALADPCMloop *)((u32)ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop + (u32)pptr);
            }
            if (ptrfile_addr->wave_list[i]->type == AL_ADPCM_WAVE) {
                ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.book =
                    (ALADPCMBook *)((u32)ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.book + (u32)pptr);
            }
        }
    }

    osWritebackDCacheAll();
}

s32 __MusIntRandom(s32 range) {
    s32 temp;
    s32 x;
    f32 f;

    for (x = 0; x < 8; x++) {
        temp = mus_random_seed & 0x48000000;
        mus_random_seed = mus_random_seed << 1;
        if (temp == 0x48000000 || temp == 0x08000000) {
            mus_random_seed = mus_random_seed | 1;
        }
    }
    f = (f32)(mus_random_seed) / (1 << 16);
    f /= (1 << 16);
    return (s32)((f32)(range)*f);
}

void __MusIntInitialiseChannel(channel_t *cp) {
    u8 old_playing;
    u8 *work_ptr;
    u32 i;
    s16 tempo;
    fx_header_t *bank;

    old_playing = cp->playing;

    work_ptr = (u8 *)cp;
    i = 0;
    cp->pdata = 0;

    do {
        *work_ptr = 0;
        work_ptr++;
        i++;
    } while (i < 0x134);

    cp->old_reverb = 0xff;
    cp->old_pan = 0xff;
    tempo = (96 * 256) / mus_vsyncs_per_second;
    cp->old_frequency = 99.9f;
    cp->pitchbend_precalc = 0.03125f;
    cp->env_attack_calc = 1.0f;
    cp->env_decay_calc = 0.00392156862745098f;
    cp->env_release_calc = 0.06666666666666667f;
    cp->velocity_on = 0;
    cp->default_velocity = 127;
    cp->volume = 127;
    cp->env_max_vol = 127;
    cp->pan = 64;
    cp->env_speed = 1;
    cp->env_decay_speed = 0xff;
    cp->env_sustain_vol = 127;
    cp->old_volume = 0xffff;
    cp->env_attack_speed = 1;
    cp->env_release_speed = 15;
    bank = libmus_fxheader_single;
    cp->length = 1;
    cp->cont_vol_repeat_count = 1;
    cp->cont_pb_repeat_count = 1;
    cp->stopping = -1;
    cp->volscale = 0x80;
    cp->panscale = 0x80;
    cp->temscale = 0x80;
    cp->playing = old_playing;
    cp->sample_bank = bank;
    cp->channel_tempo_save = tempo;
    cp->channel_tempo = tempo;
}

s32 func_800744EC_750EC(song_t *arg0, s32 arg1) {
    channel_t *cp;
    s32 i;
    s32 lowest_priority;
    s32 best_channel;

    // find an empty channel
    cp = mus_channels;
    for (i = 0; i < max_channels; i++) {
        if (cp->pdata == NULL) {
            return i;
        }
        cp++;
    }
    cp = mus_channels;

    // find lowest priority channel with fx
    lowest_priority = 0x7FFFFFFF;
    best_channel = -1;
    for (i = 0; i < max_channels; i++) {
        if (cp->fx_addr == 0) {
        } else if (lowest_priority < cp->priority) {
        } else {
            lowest_priority = cp->priority;
            best_channel = i;
        }
        cp++;
    }

    if (best_channel != -1) {
        return best_channel;
    }

    // return i if fx_addr == 0 and song_addr != arg0
    cp = mus_channels;
    i = 0;
    for (i = 0; i < max_channels; i++) {
        if (cp->fx_addr != 0) {
        } else if (cp->song_addr != arg0) {
            return i;
        }
    }

    // The reload happens unconditionally at this point
    cp = mus_channels;

    // find channel with same song and same channel data
    for (i = 0; i < max_channels; i++) {
        if (cp->song_addr != arg0) {
        } else if (arg0->channel_data[arg1] == cp->pbase) {
            return i;
        }
        cp++;
    }

    return arg1 % max_channels;
}

void __MusIntMemSet(void *dest, unsigned char value, int length) {
    unsigned char *a;

    a = dest;
    while (length--) {
        *a++ = value;
    }
}

void __MusIntMemMove(u8 *arg0, u8 *arg1, s32 arg2) {
    while (--arg2 != -1) {
        *arg1++ = *arg0++;
    }
}

void __MusIntRemapPtrs(void *addr, void *offset, s32 count) {
    u32 *dest, add;
    s32 i;

    dest = (unsigned long *)addr;
    add = (unsigned long)offset;
    for (i = 0; i < count; i++) {
        if (dest[i]) {
            dest[i] += add;
        }
    }
}

u32 __MusIntStartEffect(channel_t *cp, s32 number, s32 volume, s32 pan, s32 priority) {
    u32 handle;
    u8 *fx_data;

    __MusIntInitialiseChannel(cp);
    handle = mus_current_handle;
    cp->fx_addr = number;
    cp->volscale = volume;
    cp->panscale = pan;
    cp->handle = handle;
    cp->priority = priority;
    mus_current_handle = handle + 1;
    fx_data = D_800A64F4_A70F4[number].fxdata;
    cp->pbase = fx_data;
    cp->pdata = fx_data;
    return cp->handle;
}

void __MusIntHandleSetFlag(u32 handle, u32 clear, u32 set) {
    s32 i;
    channel_t *cp;

    for (i = 0, cp = mus_channels; i < max_channels; i++, cp++) {
        if (cp->handle == handle) {
            cp->flags &= clear;
            cp->flags |= set;
        }
    }
}
