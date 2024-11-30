
/*********************************************************

  player_fx.h : Nintendo 64 Music Tools Programmers Library

  Version 3.14

  Audio library effect support functions.

**********************************************************/

#ifndef _PLAYER_FX_H_
#define _PLAYER_FX_H_

#include "synthInternals.h"


#ifdef SUPPORT_NAUDIO
#include <n_libaudio_sc.h>
#include <n_libaudio_sn_sc.h>
#include "n_synthInternals.h"
#endif

/* public interface to change effects */
int ChangeCustomEffect(s32);

/* internal stuff */
void CustomInit(ALGlobals *, ALSynConfig *);						// replacement for alInit()

#ifndef SUPPORT_NAUDIO
#define alInit CustomInit
void CustomSynNew(ALSynth *, ALSynConfig *);						// replacement for alSynNew()
ALFxRef *CustomAllocFX(ALSynth *, s16, ALSynConfig *, ALHeap *);	// replacement for alSynAllocFX()
#else
#define n_alInit CustomInit
void CustomSynNew(ALSynConfig *);						// replacement for alSynNew()
ALFxRef CustomAllocFX(s16, ALSynConfig *, ALHeap *);	// replacement for alSynAllocFX()
#endif

/* stuff I've added/altered */
#ifndef SUPPORT_NAUDIO
void CustomFxNew(ALFx *, ALSynConfig *, ALHeap *);
#else
void CustomFxNew(ALFx **, ALSynConfig *, ALHeap *);
#endif
void CustomFxSet(s32 *);


typedef struct 
{
/* the synthesizer we;re attached to */
		ALSynth *synth;

/* effect description */
		int sections;
		u32 length;
		s32 *pParams;

/* AlFx object attached to the bus */
		ALDelay  *pDelay;
		s16      *pBase;

/* LowPass Filter */
		ALResampler *pResampler;
		RESAMPLE_STATE *pResampleState;
		ALLowPass *pLowPass;
		POLEF_STATE *pLpfState;

} REVERB_MEM;

#endif /* _PLAYER_FX_H_ */

/*** end of file ***/
