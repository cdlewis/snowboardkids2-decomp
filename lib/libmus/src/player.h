
/*********************************************************

  player.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music player header file.

**********************************************************/

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "libmus.h"
#include "libmus_data.h"

/* maximum depth of for-next loops */
#define FORNEXT_DEPTH	4

/* maximum extra channels allocated for song master tracks */
#define MAX_SONGS		4 /* if one of these channels isn't available sound generating channel is used! */

typedef struct
{
	unsigned short	wave;	/* wave number */
	unsigned short	adsr;	/* envelope number */
	unsigned char	pan;	/* pan position */
	unsigned char	pitch;	/* semitone */
} drum_t;

typedef struct 
{
	unsigned long	version;		/* 16.16 version number */
	long			num_channels;	/* number of channels in this song */
	long			num_waves;		/* number of waves in table */

	/* offsets converted to pointers go here - changing these requires
	   MusStartSong() to be changed! */
#define SONGHDR_COUNT	7					/* number of pointers to convert */
#define SONGHDR_ADR(x)	(&(x)->data_list)	/* start address of pointer list */

	/* channel pointers */
	unsigned char 	**data_list;	/* channel data address list */
	unsigned char	**volume_list;	/* channel volume address list */
	unsigned char	**pbend_list;	/* pitchbend address list */
	/* table pointers */
	unsigned char	*env_table;		/* address of envelope table */
	drum_t			*drum_table;	/* address of drum map table */
	unsigned short	*wave_table;	/* address of waveform lookup table */
	/* master track */
	unsigned char	*master_track;	/* address of master track */

	/* library workspace */
	unsigned long	flags;
	unsigned long 	reserved1;
	unsigned long	reserved2;
	unsigned long	reserved3;
} song_t;

/* standard Music Tools binary sound effect entry */
typedef struct
{
	unsigned char *fxdata;
	int		priority;
} fx_t;

/* standard Music Tools binary sound effect header */
typedef struct
{
	int			number_of_components;
	int			number_of_effects;
	int			num_waves;
	unsigned long 	flags;
	ptr_bank_t		*ptr_addr;
	unsigned short	*wave_table;
	fx_t     		effects[1];		/* "number_of_components" sized array */
} fx_header_t;


/* flags for sample banks */
#define PTRFLAG_REMAPPED	(1<<31)

/* flags for overall channel control */
#define CHFLAG_PAUSE		(1<<0)
#define CHFLAG_MASTERTRACK	(1<<1)

/* flags for sound effect banks */
#define FXFLAG_INITIALISED	(1<<0)

/* flags for songs */
#define SONGFLAG_INITIALISED	(1<<0)



/* music player channel structure */
typedef struct
{
  /* 32bit values... */
  unsigned long		channel_flag; /* new flag for overall channel control */
  unsigned char		*pdata;
  ALWaveTable		*pending;
  unsigned long		channel_frame;

  int     			stopping;
  unsigned long		volume_frame;
  unsigned long		pitchbend_frame;
  int    			stopping_speed;

  float				vib_amount;
  float				pitchbend_precalc;
  float		        old_frequency;
  float				base_note;

  float				freqoffset;
  unsigned char		*ppitchbend;
  unsigned char		*pvolume;
  unsigned long		note_end_frame;

  unsigned long		note_start_frame;
  unsigned long		handle;
  int  				priority;
  float				last_note;

  float				port_base;
  unsigned long		release_frame;
  float				env_attack_calc;
  float				env_decay_calc;

  float				env_release_calc;
  int				env_speed_calc;
  float				vibrato;
  float				bendrange;

  float				pitchbend;
  song_t			*song_addr;
  fx_header_t		*fx_addr;
  ptr_bank_t		*sample_bank;

  unsigned char		*pbase;
  drum_t 			*pdrums;
  unsigned char		*ppitchbendbase;
  unsigned char		*pvolumebase;

  float				distort;
  unsigned long		sweep_frame;  
  // 16bit values...
  short				temscale;
  unsigned short	length;
  unsigned short	channel_tempo;
  short				volscale;

  unsigned short	old_volume;
  unsigned short	cont_vol_repeat_count;
  unsigned short	cont_pb_repeat_count;
  unsigned short	fx_number;
  unsigned short	channel_tempo_save;
  unsigned short	count;
  unsigned short	fixed_length;
  unsigned short	wave;

  short				panscale;
  unsigned short	cutoff;
  unsigned short	endit;
  // 8bit values...
  unsigned char		vib_delay;
  unsigned char		ignore;

  unsigned char		port;
  unsigned char		transpose;
  unsigned char		ignore_transpose;
  unsigned char		velocity;
  unsigned char		volume;
  unsigned char		pan;
  unsigned char		old_pan;
  unsigned char		env_speed;
  unsigned char		env_init_vol;
  unsigned char		env_max_vol;
  unsigned char		env_sustain_vol;
  unsigned char		env_phase;
  unsigned char		env_current;
  unsigned char		env_count;
  unsigned char		env_attack_speed;
  unsigned char		env_decay_speed;

  unsigned char		env_release_speed;
  unsigned char		playing;
  unsigned char		reverb;			// wet/dry mix for alSynSetMix()
  unsigned char		reverb_base;
  unsigned char		old_reverb;
  unsigned char		release_start_vol;
  unsigned char		wobble_on_speed;
  unsigned char		wobble_off_speed;
  unsigned char		wobble_count;
  signed char		wobble_current;
  unsigned char		velocity_on;
  unsigned char		default_velocity;
  unsigned char		sweep_speed;
  unsigned char		vib_speed;/* zero if not active*/
  unsigned char		env_trigger_off;
  unsigned char		trigger_off;

  signed char		wobble_amount;
  unsigned char		sweep_timer;
  unsigned char		sweep_dir;
  unsigned char		for_stack_count;

  float			vib_precalc;

  /* for-next stuff all together */
  unsigned char		*for_stack	[FORNEXT_DEPTH];
  unsigned char		*for_stackvol	[FORNEXT_DEPTH];
  unsigned char		*for_stackpb	[FORNEXT_DEPTH];
  unsigned short	for_vol_count	[FORNEXT_DEPTH];
  unsigned short	for_pb_count	[FORNEXT_DEPTH];
  unsigned char		for_count	[FORNEXT_DEPTH];	
  unsigned char		for_volume	[FORNEXT_DEPTH];
  float		        for_pitchbend	[FORNEXT_DEPTH];

#ifndef SUPPORT_EFFECTS
  unsigned char padding[4]; /* pad for data cache */
#else
  unsigned long		effect_type;
  float			specialvib_amount;
  unsigned char		specialvib_speed;
  unsigned char		specialvib_delay;
  unsigned char		last_volume;
  unsigned char		vol_effect[4];
  unsigned char		pitch_effect[4];
#endif
} channel_t;


/* sound data command function prototype */
typedef	struct
{
  unsigned char *(*func)(channel_t *cp, unsigned char *ptr);
} command_func_t;



#endif /* _PLAYER_H_ */



