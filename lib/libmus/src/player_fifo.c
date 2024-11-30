
/*************************************************************

  player_fifo.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library fifo functions.

**************************************************************/


static int fifo_start;
static int fifo_current;
static int fifo_limit;

static fifo_t *fifo_addr;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntFifoOpen(commands)

  [Parameters]
  commands	number of commands to support in fifo

  [Explanation]
  Initialise fifo command buffer. The fifo command buffer is used to pass commands
  to the player from the current thread, these commands are commands that would 
  suffer it the current thread was interrupted by the players thread.

  The number of commands supported is set in the library config structure.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntFifoOpen(int commands)
{
  if (commands<MIN_FIFO_COMMANDS)
  {
#ifdef _AUDIODEBUG
    osSyncPrintf("PLAYER_FIFO.C: Using minimum fifo size.\n");
#endif
    commands = MIN_FIFO_COMMANDS;
  }
  else if (commands>MAX_FIFO_COMMANDS)
  {
#ifdef _AUDIODEBUG
    osSyncPrintf("PLAYER_FIFO.C: Using maximum fifo size.\n");
#endif
    commands = MAX_FIFO_COMMANDS;
  }

  fifo_addr = __MusIntMemMalloc(commands*sizeof(fifo_t));

  fifo_limit = commands;
  fifo_start = fifo_current = 0;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntFifoProcess()

  [Explanation]
  Process any commands in the fifo buffer, these commands will be processed before
  the player is updated.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntFifoProcess(void)
{
  if (fifo_start==fifo_current)
    return;

  while (fifo_start!=fifo_current)
  {
    __MusIntFifoProcessCommand(&fifo_addr[fifo_start]);
    fifo_start++;
    if (fifo_start==fifo_limit)
      fifo_start=0;
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntFifoProcessCommand(command)

  [Parameters]
  command	address of fifo command

  [Explanation]
  Process the specified fifo command.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntFifoProcessCommand(fifo_t *command)
{
  switch (command->command)
  {
      case FIFOCMD_PAUSE:
	__MusIntHandleSetFlag(command->data, ~CHFLAG_PAUSE, CHFLAG_PAUSE);
	break;
      case FIFOCMD_UNPAUSE:
	__MusIntHandleSetFlag(command->data, ~CHFLAG_PAUSE, 0);
	break;
      case FIFOCMD_CHANGEFX:
#ifdef SUPPORT_FXCHANGE
	ChangeCustomEffect(command->data);
#endif
	break;
#ifdef _AUDIODEBUG
      default:
	osSyncPrintf("PLAYER_FIFO.C: Unknown fifo command found!\n");
#endif
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntFifoAddCommand(command)

  [Parameters]
  command	address of fifo command

  [Explanation]
  Store the given command in the fifo list.

  [Return value]
  0 if the fifo list is full, <>0 if the command was stored.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static int __MusIntFifoAddCommand(fifo_t *command)
{
  int index;

  index = (fifo_current+1)%fifo_limit;
  if (index==fifo_start)
  {
#ifdef _AUDIODEBUG
    osSyncPrintf("PLAYER_FIFO.C: Fifo is full ignoring last command!\n");
#endif
    return (0);
  }
  __MusIntMemMove(&fifo_addr[fifo_current], command, sizeof(fifo_t));
  fifo_current = index;
  return (1);
}

/* end of file */
