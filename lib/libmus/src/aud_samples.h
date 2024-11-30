
/*********************************************************

  aud_samples.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.
  
  Version 3.14

  Music library frame sample calculations.

**********************************************************/

#ifndef _LIBMUS_AUD_SAMPLES_H_
#define _LIBMUS_AUD_SAMPLES_H_


u32 __MusIntSamplesInit		(u32 retrace_count, u32 output_rate, u32 vsyncs_per_sec, u32 extra_rate);
u32 __MusIntSamplesCurrent	(u32 samples);


#endif /* _LIBMUS_AUD_SAMPLES_H_ */

/* end of file */
