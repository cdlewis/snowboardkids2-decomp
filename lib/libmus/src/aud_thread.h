
/*********************************************************

  aud_thread.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library thread base audio manager.

**********************************************************/

#ifndef _LIBMUS_AUD_THREAD_H_
#define _LIBMUS_AUD_THREAD_H_


/* global vars */
extern ALGlobals   __libmus_alglobals;

/* function prototypes */
void   __MusIntAudManInit(musConfig *config, int vsyncs_per_second, int fx_type);


#endif /* _LIBMUS_AUD_THREAD_H_ */

/* end of file */
