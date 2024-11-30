
/*====================================================================
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

/***************************************************************

  player_fx.c : Nintendo 64 Music Tools Programmers Library

  Version 3.14

  Audio library FX manager.

****************************************************************/


#include "libmus_config.h"

#ifdef SUPPORT_FXCHANGE

#include <ultra64.h>
#include <sched.h>
#include "player_fx.h"

/* this global structure handles memory within the Audio Heap that is used by the Effect object */
REVERB_MEM gReverbFx;

/* internal util function prototypes */
static s32 GetFxMostSections(void);
static s32 GetFxLongest(void);
static void SetForCustom(ALSynConfig *c);

#ifdef SUPPORT_NAUDIO
#define alGlobals n_alGlobals
#endif

/* same as alInit(), but passes to CustomSynNew() */
void CustomInit(ALGlobals *g, ALSynConfig *c)
{
  if (!alGlobals)	/* already initialized? */
  {
    alGlobals = g;
#ifndef SUPPORT_NAUDIO
    SetForCustom(c);
    gReverbFx.synth = &alGlobals -> drvr;	// we'll need it later
    CustomSynNew(&alGlobals->drvr, c);
#else
    if(!n_syn)
    {
      SetForCustom(c);
      n_syn = gReverbFx.synth = &alGlobals -> drvr;	// we'll need it later
      CustomSynNew(c);
    }
#endif
  }
}
/* same as alSynNew(), but passes to CustomSynAllocFx() */
#ifndef SUPPORT_NAUDIO
void CustomSynNew(ALSynth *drvr, ALSynConfig *c)
{
    s32         i;
    PVoice      *pv;
    PVoice      *pvoices;
    ALHeap      *hp = c->heap;
    ALSave      *save;
    ALFilter    *sources;
    ALParam     *params;
    ALParam     *paramPtr;
    
    drvr->head            = NULL;

    drvr->numPVoices      = c->maxPVoices;
    drvr->curSamples      = 0;
    drvr->paramSamples    = 0;
    drvr->outputRate      = c->outputRate;
    drvr->maxOutSamples   = AL_MAX_RSP_SAMPLES;
    drvr->dma             = (ALDMANew) c->dmaproc;

    save = alHeapAlloc(hp, 1, sizeof(ALSave));
    alSaveNew(save);
    drvr->outputFilter = (ALFilter *)save;

    /*
     * allocate and initialize the auxilliary effects bus. at present
     * we only support 1 effects bus.
     */
    drvr->auxBus = alHeapAlloc(hp, 1, sizeof(ALAuxBus));
    drvr->maxAuxBusses = 1;
    sources = alHeapAlloc(hp, c->maxPVoices, sizeof(ALFilter *));
    alAuxBusNew(drvr->auxBus, sources, c->maxPVoices);

    /*
     * allocate and initialize the main bus.
     */
    drvr->mainBus = alHeapAlloc(hp, 1, sizeof(ALMainBus));
    sources = alHeapAlloc(hp, c->maxPVoices, sizeof(ALFilter *));
    alMainBusNew(drvr->mainBus, sources, c->maxPVoices);

    if (c->fxType != AL_FX_NONE)
    {
        /*
         * Allocate an effect and set parameters
         */
        CustomAllocFX(drvr, 0, c, hp);
	}
    else
    {
        /*
         * Connect the aux bus to the main bus
         */
    	alMainBusParam(drvr->mainBus, AL_FILTER_ADD_SOURCE, &drvr->auxBus[0]);
    }

    /*
     * Build the physical voice lists
     */
    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;

    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        alLink((ALLink *)pv, &drvr->pFreeList);
        pv->vvoice = 0;

        alLoadNew(&pv->decoder, drvr->dma, hp);
        alLoadParam(&pv->decoder, AL_FILTER_SET_SOURCE, 0);

        alResampleNew(&pv->resampler, hp);
        alResampleParam(&pv->resampler, AL_FILTER_SET_SOURCE, &pv->decoder);

        alEnvmixerNew(&pv->envmixer, hp);
        alEnvmixerParam(&pv->envmixer, AL_FILTER_SET_SOURCE, &pv->resampler);

        alAuxBusParam(drvr->auxBus, AL_FILTER_ADD_SOURCE, &pv->envmixer);

        pv->channelKnob   = (ALFilter *)&pv->envmixer;
    }

    alSaveParam(save, AL_FILTER_SET_SOURCE, drvr->mainBus);

    /*
     * build the parameter update list
     */
    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++)
    {
        paramPtr= &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }
    
    drvr->heap = hp;
}
#else
void CustomSynNew(ALSynConfig *c)
{
    s32         i;
    N_PVoice   *pv;
    N_PVoice   *pvoices;
    ALHeap      *hp = c->heap;
    ALParam     *params;
    ALParam     *paramPtr;

    n_syn->head            = NULL;
    n_syn->numPVoices      = c->maxPVoices;

    n_syn->curSamples      = (s32)0;
    n_syn->paramSamples    = (s32)0;
    n_syn->outputRate      = c->outputRate;

// #ifndef N_MICRO
//     n_syn->maxOutSamples   = AL_MAX_RSP_SAMPLES;
// #else
    n_syn->maxOutSamples   = FIXED_SAMPLE;
// #endif
    n_syn->dma             = (ALDMANew) c->dmaproc;

/******* save new *******************************/
    n_syn->sv_dramout = 0;
    n_syn->sv_first = 1;

/******* aux new *******************************/
    n_syn->auxBus = (N_ALAuxBus *)alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));

    n_syn->auxBus->sourceCount = 0;
    n_syn->auxBus->maxSources = c->maxPVoices;
    n_syn->auxBus->sources = (N_PVoice **)
      alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));

/******* main new *******************************/
    n_syn->mainBus = (N_ALMainBus *)alHeapAlloc(hp, 1, sizeof(N_ALMainBus));

/******* fx new *******************************/

    if (c->fxType != AL_FX_NONE){
      n_syn->auxBus->fx = CustomAllocFX(0, c, hp);
      n_syn->mainBus->filter.handler = (N_ALCmdHandler)n_alFxPull;
    } else {
      n_syn->mainBus->filter.handler = (N_ALCmdHandler)n_alAuxBusPull;
    }

    n_syn->pFreeList.next = 0;
    n_syn->pFreeList.prev = 0;
    n_syn->pLameList.next = 0;
    n_syn->pLameList.prev = 0;
    n_syn->pAllocList.next = 0;
    n_syn->pAllocList.prev = 0;

    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
      pv = &pvoices[i];
      alLink((ALLink *)pv, &n_syn->pFreeList);
      pv->vvoice = 0;
      alN_PVoiceNew(pv, n_syn->dma, hp);
      n_syn->auxBus->sources[n_syn->auxBus->sourceCount++] = pv;
    }

    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    n_syn->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr= &params[i];
        paramPtr->next = n_syn->paramList;
        n_syn->paramList = paramPtr;
    }
    n_syn->heap = hp;

}
#endif

/* same as alSynAllocFX but calls modified functions */
#ifndef SUPPORT_NAUDIO
ALFxRef *CustomAllocFX(ALSynth *s, s16 bus, ALSynConfig *c, ALHeap *hp)
{
	CustomFxNew(&s->auxBus[bus].fx[0], c, hp);
	alFxParam(&s->auxBus[bus].fx[0], AL_FILTER_SET_SOURCE,&s->auxBus[bus]);	/* sets source for the FX */
	alMainBusParam(s->mainBus, AL_FILTER_ADD_SOURCE,&s->auxBus[bus].fx[0]);	/* Adds a source to the Main Bus */

	return (ALFxRef)(&s->auxBus[bus].fx[0]);
}
#else
ALFxRef CustomAllocFX(s16 bus, ALSynConfig *c, ALHeap *hp)
{
    CustomFxNew(&n_syn->auxBus->fx_array[bus], c, hp);
    return(n_syn->auxBus->fx_array[bus]);
}
#endif

/*
 *	create a new effect
 *
 *	this has been modified to keep the pointers to the used memory
 *	so we can modify and re-use later
 *	Because of this the actual setting of the filter etc. is done in CustomFxSet()
 */

#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH	(d->output - d->input)

#ifndef SUPPORT_NAUDIO
void CustomFxNew(ALFx *r, ALSynConfig *c, ALHeap *hp)
{
    ALFilter	*f = (ALFilter *)r;

    alFilterNew(f, 0, alFxParam, AL_FX);
    f->handler = alFxPull;
#else
void CustomFxNew(ALFx **fx_ar, ALSynConfig *c, ALHeap *hp)
{
    ALFx *r;

    *fx_ar = r = (ALFx *)alHeapAlloc(hp, 1, sizeof(ALFx));
#endif

    r->paramHdl = (ALSetFXParam)alFxParamHdl;

	/* allocate enough space for the biggest effect */
    gReverbFx.pDelay = alHeapAlloc(hp, GetFxMostSections(), sizeof(ALDelay));
    gReverbFx.pBase = alHeapAlloc(hp, GetFxLongest(), sizeof(s16));

	/* allocate some stuff used by the Delay Lines */
	gReverbFx.pResampler = alHeapAlloc(hp, 1, sizeof(ALResampler));
	gReverbFx.pResampleState = alHeapAlloc(hp, 1, sizeof(RESAMPLE_STATE));
	gReverbFx.pLowPass = alHeapAlloc(hp, 1, sizeof(ALLowPass));
	gReverbFx.pLpfState = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));

	/* set it up */
	CustomFxSet(c->params);
}

/*
 *	set up all the stuff for an effect
 *
 *	This behaves as if AL_FX_CUSTOM was set as the type (without checking the type)
 *	the params member of the ALSynConfig structure must have been set correctly
 *	before calling CustomInit(), otherwise the param entry parameter will be nonsense
 */

/* SGI library internal function prototype */
#ifdef _OLD_AUDIO_LIBRARY
#ifndef SUPPORT_NAUDIO
void init_lpfilter(ALLowPass *lp);
#endif
#endif 

void CustomFxSet(s32 *param)
{
	u16	i, j;
#ifndef SUPPORT_NAUDIO
	ALFx *r = &gReverbFx.synth->auxBus[0].fx[0];
#else
        ALFx *r = gReverbFx.synth->auxBus->fx_array[0];
#endif

	j = 0;

    r->section_count = param[j++];
    r->length 	     = param[j++];

    r->delay = gReverbFx.pDelay;
    r->base = gReverbFx.pBase;
    r->input = r->base;

	/* clear the filter outputs (??)*/
    for(i = 0; i < r->length; i++)
		r->base[i] = 0;

    for(i = 0; i < r->section_count; i++)
    {
		ALDelay *d = &r->delay[i];
		d->input  = param[j++];
		d->output = param[j++];
		d->fbcoef = param[j++];
		d->ffcoef = param[j++];
		d->gain   = param[j++];

		if (param[j])
		{
		    d->rsinc = ((((f32)param[j++])/1000) * RANGE) / gReverbFx.synth->outputRate;

			/*
			 * the following constant is derived from:
			 *
			 *		ratio = 2^(cents/1200)
			 *
			 * and therefore for hundredths of a cent
			 *			           x
			 *		ln(ratio) = ---------------
			 *			    (120,000)/ln(2)
			 * where
			 *		120,000/ln(2) = 173123.40...
			 */

			d->rsgain 	 = (((f32) param[j++])/CONVERT) * LENGTH;
			d->rsval	 = 1.0;
			d->rsdelta	 = 0.0;
			d->rs 	 = gReverbFx.pResampler;
			d->rs->state = gReverbFx.pResampleState;
			d->rs->delta = 0.0;
			d->rs->first = 1;
		}
		else
		{
			d->rs = 0;
			j++;
			j++;
		}

		if (param[j])
		{
			d->lp = gReverbFx.pLowPass;
			d->lp->fstate = gReverbFx.pLpfState;
			d->lp->fc = param[j++];
#ifdef _OLD_AUDIO_LIBRARY
			init_lpfilter(d->lp);
#else
			_init_lpfilter(d->lp);
#endif
		}
		else
		{
			d->lp = 0;
			j++;
		}
    }
}

/**********************************************************************************************/
/**********************************************************************************************/

/*
 *	copied Effects from SGI library, which we can use as custom effects
 */

#define ms *(((s32)((f32)44.1))&~0x7)

static s32 SmallRoomParams[26] = {
    /* sections	   length */
          3,        100 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef  */
           0,   54 ms,  9830,  -9830,      0,      0,      0,      0,
       19 ms,   38 ms,  3276,  -3276, 0x3fff,      0,      0,      0,
           0,   60 ms,  5000,      0,      0,      0,      0, 0x5000,
};

static s32 BigRoomParams[34] = {
    /* sections	   length */
          4,        100 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef  */
           0,   66 ms,  9830,  -9830,      0,      0,      0,      0,
       22 ms,   54 ms,  3276,  -3276, 0x3fff,      0,      0,      0,
       66 ms,   91 ms,  3276,  -3276, 0x3fff,      0,      0,      0,
           0,   94 ms,  8000,      0,      0,      0,      0, 0x5000
};

static s32 EchoParams[10] = {
    /* sections	   length */
          1,       200 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
           0,  179 ms, 12000,      0, 0x7fff,      0,      0,      0,
};

static s32 ChorusParams[34] = {
    /* sections	   length */
          1,        20 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
	  0,   5 ms, 0x4000,      0,  0x7fff,   7600,   700,      0
};

static s32 FlangeParams[10] = {
    /* sections	   length */
          1,        20 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
	   0,   5 ms,      0, 0x5fff, 0x7fff,    380,   500,      0
};

static s32 NoFxParams[10] = {
    /* sections	   length */
          1,        20 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
	   0,   5 ms,      0, 0,0,    0,   0,      0
};


/*
 *	this will be temporary, until we have editable effects - TW
 */

// a list of all available effects
// will eventually be part of a project (oh please, NOT part of a song)

static s32 EffectCount = 6;		/* number of effects in the list */

static s32 *EffectList[] = {
	NoFxParams,
	SmallRoomParams,
	BigRoomParams,
	ChorusParams,
    FlangeParams,
	EchoParams,
	NULL
};

/*
 *	find the most sections used by an effect
 *	this is used to allocate space for Delay Lines in the AlFX object
 *
 */

static void SetForCustom(ALSynConfig *c)
{
  c->fxType = AL_FX_CUSTOM;
  c->params = BigRoomParams;
}

static s32 GetFxMostSections(void)
{
	int i;
	s32 most = 0;

	for(i = 0; EffectList[i] != NULL; i++)
	{
		s32 *pFx = EffectList[i];

		/* the first s32 is the number of sections */
		if(pFx[0] > most)
		{
			most = pFx[0];
		}
	}

	return most;
}

/*
 *	find the length of the longest effect
 *	this is important as it means we only allocate once from the Audio Library Heap
 *	memory cannot be freed from an Audio Heap
 */

static s32 GetFxLongest(void)
{
	int i;
	s32 longest = 0;

	for(i = 0; EffectList[i] != NULL; i++)
	{
		s32 *pFx = EffectList[i];

		/* the second s32 is the length of the effect */
		if(pFx[1] > longest)
		{
			longest = pFx[1];
		}
	}

	return longest;
}

/*
 *	Change to a new effect setup
 *
 *	N.B. this needn't be called if you only set some parameters within an effect
 *		 there are library functions to do that, but the library functions
 *		 don't allow you to change length or number of sections within an effect
 *
 *		param is a list of s32 values, as per SGI documentation
 */

int ChangeCustomEffect(s32 type)
{
	/* IMPORTANT - the Audio Thread MUST NOT be generating any audio when this is called */
	/* e.g. call PcRomSilence(), then wait for retrace to make sure all audio is used up */

	if(type >= EffectCount)
		return(1);		/* index is larger than list */

	CustomFxSet(EffectList[type]);

	alFxParam(&gReverbFx.synth->auxBus[0].fx[0], AL_FILTER_SET_SOURCE,&gReverbFx.synth->auxBus[0]);	/* sets source for the FX */

	return(0);			/* done ok */
}

#endif /* SUPPORT_FXCHANGE */

/*** end of file ***/
