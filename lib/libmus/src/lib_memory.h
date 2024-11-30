
/*********************************************************

  lib_memory.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library memory manager.

**********************************************************/

#ifndef _LIBMUS_LIB_MEMORY_H_
#define _LIBMUS_LIB_MEMORY_H_


/* function prototypes */
void	__MusIntMemInit			(void *addr, int length);
ALHeap	*__MusIntMemGetHeapAddr	(void);
void	*__MusIntMemMalloc		(int length);
int 	__MusIntMemRemaining	(void);

void	__MusIntMemSet			(void *dest, unsigned char value, int length);
void	__MusIntMemMove			(void *dest, void *src, int length);


#endif	/* _LIBMUS_LIB_MEMORY_H_ */


/* end of file */
