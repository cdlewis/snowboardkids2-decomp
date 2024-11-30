
/*********************************************************

  aud_sched.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library scheduler functions.

**********************************************************/

#ifndef _LIBMUS_AUD_SCHED_H_
#define _LIBMUS_AUD_SCHED_H_


/* global vars */
extern musSched *__libmus_current_sched;

/* function prototypes */
void	__MusIntSchedInit		(void *sched);

/* function prototypes - implemented as macros below */
void	__MusIntSched_install	(void);
void	__MusIntSched_waitframe	(void);
void	__MusIntSched_dotask	(musTask *task);


#define __MusIntSched_install()		__libmus_current_sched->install()
#define __MusIntSched_waitframe()	__libmus_current_sched->waitframe()
#define __MusIntSched_dotask(task)	__libmus_current_sched->dotask((task))


#endif /* _LIBMUS_AUD_SCHED_H_ */

/* end of file */
