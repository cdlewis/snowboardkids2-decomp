
/*********************************************************

  libmus.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library header file.

**********************************************************/

#ifndef _LIBMUS_H_
#define _LIBMUS_H_

#include <ultra64.h>

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

/* --------- MUSIC PLAYER API TYPEDEFS --------- */

/* configuration */
typedef struct 
{
  unsigned long control_flag;

  int 		channels;
  void		*sched;
  int		thread_priority;
  unsigned char	*heap;
  int		heap_length;

  unsigned char	*ptr;
  unsigned char	*wbk;

  void		*default_fxbank;

  int		fifo_length;

  int		syn_updates;
  int		syn_output_rate;
  int		syn_rsp_cmds;
  int 		syn_retraceCount;
  int		syn_num_dma_bufs;
  int		syn_dma_buf_size;

// Special Addition
  OSPiHandle	*diskrom_handle;
} musConfig;

/* enabled/disable flag */
typedef enum
{
  MUSBOOL_OFF,
  MUSBOOL_ON
} musBool;

/* task descriptor */
typedef struct
{
	u64		*data;				/* address of Acmd list			*/
	int		data_size;			/* size of Acmd list			*/
	u64		*ucode;				/* address of microcode code	*/
	u64		*ucode_data;		/* address of microcode data	*/
} musTask;

/* handle type */
typedef unsigned long	musHandle;

/* marker callback function */
typedef void	(*LIBMUScb_marker)		(musHandle, int);

/* scheduler support callback functions */
typedef void	(*LIBMUScb_install)		(void);
typedef void	(*LIBMUScb_waitframe)	(void);
typedef void 	(*LIBMUScb_dotask)		(musTask *);

/* scheduler support callback list */
typedef struct
{
	LIBMUScb_install	install;		/* called when thread starts		*/
	LIBMUScb_waitframe	waitframe;		/* called to wait for vsync message */
	LIBMUScb_dotask		dotask;			/* called to process RSP task		*/
} musSched;


/* --------- MUSIC PLAYER API MACROS --------- */

/* control flags */
#define MUSCONTROL_RAM		(1<<0)

/* channel types flags */
#define MUSFLAG_EFFECTS	1
#define MUSFLAG_SONGS	2


/* --------- MUSIC PLAYER API FUNCTION PROTOTYPES --------- */

/* initialise */
extern int MusInitialize			(musConfig *config);

/* audio configuration */
extern int MusSetFxType			(int fxtype);
extern int MusSetSongFxChange		(musBool onoff);

/* set master volume levels */
extern void MusSetMasterVolume		(unsigned long flags, int volume);

/* start songs and sound effects */
extern musHandle MusStartSong		(void *addr);
extern musHandle MusStartSongFromMarker	(void *addr, int marker);
extern musHandle MusStartEffect		(int number);
extern musHandle MusStartEffect2		(int number, int volume, int pan , int restartflag, int priority);

/* stop and query sound types */
extern void MusStop				(unsigned long flags, int speed);
extern int  MusAsk				(unsigned long flags);

/* handle based processing */
extern int MusHandleAsk			(musHandle handle);
extern int MusHandleStop			(musHandle handle, int speed);
extern int MusHandleSetVolume		(musHandle handle, int volume);
extern int MusHandleSetPan			(musHandle handle, int pan);
extern int MusHandleSetFreqOffset		(musHandle handle, float offset);
extern int MusHandleSetTempo		(musHandle handle, int tempo);
extern int MusHandleSetReverb		(musHandle handle, int reverb);
extern int MusHandlePause			(musHandle handle);
extern int MusHandleUnPause			(musHandle handle);
extern void *MusHandleGetPtrBank		(musHandle handle);

/* sample bank support */
extern void MusPtrBankInitialize		(void *pbank, void *wbank);
extern void *MusPtrBankSetSingle		(void *ipbank);
extern void MusPtrBankSetCurrent		(void *ipbank);
extern void *MusPtrBankGetCurrent		(void);

/* sound effect bank support */
extern void MusFxBankInitialize		(void *fxbank);
extern void MusFxBankSetSingle		(void *ifxbank);
extern void MusFxBankSetCurrent		(void *ifxbank);
extern void *MusFxBankGetCurrent		(void);
extern int  MusFxBankNumberOfEffects	(void *ifxbank);
extern void MusFxBankSetPtrBank		(void *ifxbank, void *ipbank);
extern void *MusFxBankGetPtrBank		(void *ifxbank);
extern void MusFxBankSetSingle		(void *ifxbank);

/* scheduler support */
extern void MusSetScheduler			(musSched *sched_list);

/* marker callback support */
extern void MusSetMarkerCallback	(void *callback);

/* wave list lookup in song header */
extern int    			MusHandleWaveCount	(musHandle handle);
extern unsigned short	*MusHandleWaveAddress(musHandle handle);


/* macros to support previous sample bank functions - use is not recommended! */
#define MusBankInitialize(pbank,wbank)	\
	MusPtrBankInitialize(pbank,wbank)

#define MusBankStartSong(ipbank, addr)	\
	MusStartSong((addr)==(void *)MusPtrBankSetSingle(ipbank) ? (addr):(addr))

#define MusBankStartEffect(ipbank, number) \
	MusStartEffect((number)==(int)MusPtrBankSetSingle(ipbank) ? (number):(number))

#define MusBankStartEffect2(ipbank, number, volume, pan, restartflag, priority) \
	MusStartEffect2((number)==(int)MusPtrBankSetSingle(ipbank) ? (number):(number), \
					volume, pan, restartflag, priority)



/* --- DEVELOPEMENT API FUNCTION PROTOTYPES --- */



#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* _LIBMUS_H_ */
