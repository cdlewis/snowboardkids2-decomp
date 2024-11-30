
/*********************************************************

  player_fifo.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library fifo functions (only use internally!).

**********************************************************/

#ifndef _PLAYER_FIFO_H_
#define _PLAYER_FIFO_H_

typedef struct
{
  unsigned char		command;
  unsigned char		padding1;
  unsigned char		padding2;
  unsigned char		padding3;
  unsigned long		data;
} fifo_t;

/* fifo size limits */
#define MIN_FIFO_COMMANDS	(64)
#define MAX_FIFO_COMMANDS	(1024)

/* fifo commands */
enum
{
  FIFOCMD_PAUSE,
  FIFOCMD_UNPAUSE,
  FIFOCMD_CHANGEFX,
  FIFOCMD_LAST
};

static void	__MusIntFifoOpen(int commands);
static void __MusIntFifoProcess(void);
static void __MusIntFifoProcessCommand(fifo_t *command);
static int	__MusIntFifoAddCommand(fifo_t *command);


#endif /* _PLAYER_FIFO_H_ */

/* end of file */
