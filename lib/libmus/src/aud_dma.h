
/*********************************************************

  aud_dma.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library DMA buffer manager.

**********************************************************/

#ifndef _LIBMUS_AUD_DMA_H_
#define _LIBMUS_AUD_DMA_H_


ALDMANew	__MusIntDmaInit		(int dma_buffer_count, int dma_buffer_size);
void		__MusIntDmaProcess	(void);


#endif /* _LIBMUS_AUD_DMA_H_ */

/* end of file */
