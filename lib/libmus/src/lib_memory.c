
/************************************************************

  lib_memory.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library memory management functions.

*************************************************************/

/* include configuartion */
#include "libmus_config.h"

/* include system headers */
#include <ultra64.h>
#ifndef SUPPORT_NAUDIO
  #include <libaudio.h>
#else
  #include <n_libaudio_sc.h>
  #include <n_libaudio_sn_sc.h>
#endif

/* include current header file */
#include "lib_memory.h"

/* internal workspace */
static ALHeap audio_heap;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntMemInit(addr, length)

  [Parameters]
  addr			address of memory to be used as audio heap
  length		size of memory to be used as audio heap (in bytes)

  [Explanation]
  Initialise audio heap.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void __MusIntMemInit(void *addr, int length)
{
	__MusIntMemSet(addr, 0, length);
	alHeapInit(&audio_heap, addr, length);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntMemMalloc(length)

  [Parameters]
  length		number of bytes required

  [Explanation]
  Claim a number of bytes from the audio heap. This memory must be cache aligned.

  [Return value]
  Address of memory block.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void *__MusIntMemMalloc(int length)
{
	return (alHeapAlloc(&audio_heap, 1, length));
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntMemRemaining()

  [Explanation]
  Get the number of bytes remaining in the audio heap.

  [Return value]
  Number of bytes remaining in the audio heap.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

int __MusIntMemRemaining(void)
{
	return (audio_heap.cur-audio_heap.base);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntGetHeapAddr()

  [Explanation]
  Get the address of the audio heap configuration structure.

  [Return value]
  Address of audio heap configuration structure.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

ALHeap *__MusIntMemGetHeapAddr(void)
{
	return (&audio_heap);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntMemSet(addr, value, length)

  [Parameters]
  dest			address of memory block
  value			value to set
  length		size of memory block

  [Explanation]
  Set a memory block to the specified value.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void __MusIntMemSet(void *dest, unsigned char value, int length)
{
  unsigned char *a;

  a = dest;
  while (length--)
    *a++=value;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntMemMove(dest, src, length)

  [Parameters]
  dest			address of destination memory block
  src			address of source memory block
  length		size of memory block

  [Explanation]
  Move a memory block from one place to another. This function is none destructive.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void __MusIntMemMove(void *dest, void *src, int length)
{
	unsigned char *a, *b;

	a = dest;
	b = src;
	if (b<a)
	{
		a+=length;
		b+=length;
		while (length--)
			*--a = *--b;
	}
	else
	{
		while (length--)
			*a++=*b++;
	}
}


/* end of file */
