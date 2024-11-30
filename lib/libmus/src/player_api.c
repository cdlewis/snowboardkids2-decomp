
/***************************************************************

  player_api.c : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Music library Application Programming Interface. This file 
  is included in the 'player.c' file directly.

****************************************************************/


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusInitialize(config)

  [Parameters]
  config       address of configuration structure

  [Explanation]
  Initialise the music player and the audio manager.

  [Return value]
  amount of memory used from the audio heap
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/
// Special Addition
extern OSPiHandle *diskrom_handle;

int MusInitialize(musConfig *config)
{
	ALVoiceConfig vc;  
	int i;

	/* set DDROM handle */
	diskrom_handle = config->diskrom_handle;

	/* main control flag */
	__muscontrol_flag = config->control_flag;

	/* copy player settings first */
	max_channels = config->channels+MAX_SONGS;

	/* get video refresh rate */
	if (osTvType==0)
		mus_vsyncs_per_second = 50;
	else
		mus_vsyncs_per_second = 60;
	mus_next_frame_time = 1000000/mus_vsyncs_per_second;

	/* initialise heap */
	__MusIntMemInit(config->heap, config->heap_length);

	/* initialise audio scheduler functions */
	__MusIntSchedInit(config->sched);

	/* claim and clear memory for voices and channels */
	mus_voices = __MusIntMemMalloc((max_channels-MAX_SONGS)*sizeof(ALVoice));
	mus_channels = __MusIntMemMalloc(max_channels*sizeof(channel_t));
	mus_channels2 = mus_channels+MAX_SONGS;
	__MusIntFifoOpen(config->fifo_length);

	/* initialse default sample bank */
	mus_default_bank = mus_init_bank = NULL;
	if (config->ptr && config->wbk)
		MusPtrBankInitialize(config->ptr, config->wbk);

	/* initialise default sound effect bank */
	libmus_fxheader_current = libmus_fxheader_single = NULL;
	if (config->default_fxbank)
		MusFxBankInitialize(config->default_fxbank);

	/* disable marker callback function */
	marker_callback=NULL;

	/* initialise audio thread */
	mus_last_fxtype = AL_FX_BIGROOM;
   __MusIntAudManInit(config, mus_vsyncs_per_second, mus_last_fxtype);

	/* set volumes to maxiumum level */
	MusSetMasterVolume(MUSFLAG_EFFECTS|MUSFLAG_SONGS, 0x7fff);

	/* initialise player vars */
	mus_current_handle = 1;
	mus_random_seed = 0x12345678;

	/* sign into the synthesis driver */
	plr_player.next       = NULL;
	plr_player.handler    = __MusIntMain;
	plr_player.clientData = &plr_player;
	alSynAddPlayer(&__libmus_alglobals.drvr, &plr_player);
  
	/* reset channels */
	for(i=0; i<max_channels; i++)
	{
		mus_channels[i].playing = 0;
		__MusIntInitialiseChannel(&mus_channels[i]);
    
		vc.unityPitch = 0;
		vc.priority = config->thread_priority;
		vc.fxBus = 0;
		if (i>=MAX_SONGS)    
			alSynAllocVoice(&__libmus_alglobals.drvr, &mus_voices[i-MAX_SONGS], &vc);
	}
	return (__MusIntMemRemaining());
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusSetAllMasterVolumes(flags,volume)

  [Parameters]
  flags        bit pattern indicating which types to stop
  volume       volume level (0-0x7FFF)

  [Explanation]
  Set the song and sound effect master volume level.

  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusSetMasterVolume(unsigned long flags, int volume)
{
  if (flags&MUSFLAG_EFFECTS)
    mus_master_volume_effects = volume;
  if (flags&MUSFLAG_SONGS)
    mus_master_volume_songs = volume;
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartSong(addr)

  [Parameters]
  addr         address of binary song data in RAM (must be 32-bit aligned)

  [Explanation]
  This function will start the song at the given address in RAM.

  [Return value]
  sound handle
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

musHandle MusStartSong(void *addr)
{
	musHandle handle;

	handle = __MusIntStartSong(addr);
	MusHandleUnPause(handle);
	return (handle);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartSongFromMarker(addr, marker)

  [Parameters]
  addr         address of binary song data in RAM (must be 32-bit aligned)
  marker			marker number

  [Explanation]
  This function will start the song at the given address in RAM from the given 
  marker.

  NOTE:	If the marker given for the song is invalid then this function may not 
  			return.

  [Return value]
  sound handle
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

musHandle MusStartSongFromMarker(void *addr, int marker)
{
	musHandle handle;
	unsigned char command, *ptr;
	int i, note;
	channel_t *cp;

	handle = __MusIntStartSong(addr);

	/* skip to correct marker */
	for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
	{
		if (cp->handle==handle && cp->song_addr==(song_t *)addr && (cp->pdata))
		{
			/* skip to marker */
			while (cp->pdata)
			{
				/* commands must be processed */
				ptr = cp->pdata;
				if (*ptr>=128)
				{
					if (*ptr==Cmarker && *(ptr+1)==marker)
						break;

					command = *ptr++;
#ifdef _AUDIODEBUG
					if(command >= Clast)
					{
						osSyncPrintf("PLAYER_API.C: Channel %d is corrupt (command=%02x)\n",i,command);
						cp->pdata = NULL;
						break;
					}    
#endif
					cp->pdata = (jumptable[command&0x7f].func)(cp, ptr);
					continue;
				}
				note=*(cp->pdata++);
				/* velocity must be processed */
				if(cp->velocity_on)
				{
					cp->velocity = *(cp->pdata++);
					if (cp->velocity>=0x80)
					{
						cp->velocity&=0x7f;
						cp->velocity_on=0;
						cp->default_velocity = cp->velocity;
					}
				}
				else
				{
					cp->velocity = cp->default_velocity;
				}
				/* get length for continuous data update */
				if (cp->fixed_length && !cp->ignore)
				{
					cp->length = cp->fixed_length;
				}
				else
				{
					cp->ignore = 0;
					command = *(cp->pdata++);
					if (command < 128)
						cp->length = command;
					else
						cp->length = ((int)(command&0x7f)<<8)+*(cp->pdata++);
				}
				cp->channel_frame += cp->length*256;
			}

			cp->note_end_frame = cp->channel_frame;
			if (cp->pdata)
			{
				/* get marker delay vaule */
				ptr = cp->pdata+2;
				note=*ptr++;
				if (note>=0x80)
				{
					note &= 0x7f;
					note <<= 8;
					note |= *ptr++;
				}
				cp->channel_frame -= note*256;
				cp->count = 0;
				cp->length = note;
				cp->pdata = ptr;
			}
			cp->note_start_frame = cp->channel_frame;
			/* advance through volume data */
    		if(cp->pvolume)
      		__MusIntProcessContinuousVolume(cp);
			/* advance through pitchbend data */
    		if(cp->ppitchbend)
      		__MusIntProcessContinuousPitchBend(cp);
		}
	}
	MusHandleUnPause(handle);
	return (handle);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartEffect(number)

  [Parameters]
  number       sound effect number to start

  [Explanation]
  Start the specified sound effect with default parameters.

  [Return value]
  sound effect handle (0=sound effect not triggered)
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

musHandle MusStartEffect(int number)
{
   musHandle handle;
	fx_header_t *header;

	if (libmus_fxheader_single)
	{
		header = libmus_fxheader_single;
		libmus_fxheader_single = NULL;
	}
	else
	{
		header = libmus_fxheader_current;
   	/* are effects present? */
   	if (!header)
   	{
#ifdef _AUDIODEBUG
      	osSyncPrintf("PLAYER_API.C: MusStartEffect() no effect bank present.\n");
#endif
      	mus_init_bank = NULL;
      	return (0);
   	}
	}

   /* set FX default sample bank */
   if (!mus_init_bank)
      mus_init_bank = header->ptr_addr;
   /* start effect default priority */
   handle = __MusIntFindChannelAndStart(header, number, 0x80, 0x80, -1);
   /* reset any single sample bank override */
   mus_init_bank = NULL;
   return (handle);
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartEffect2(number, volume, pan, restartflag, priority)

  [Parameters]
  number       sound effect number to start
  volume       volume scale
  pan          pan scale
  restartflag  flag to control if sound effect should overwrite effects with
               the same number (0=no, <>0=yes)
  priority     sound effects priority (-1 = use Music Tools priority level)

  [Explanation]
  Start the specified sound effect with given parameters.

  [Return value]
  sound effect handle (0=sound effect not triggered)
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

musHandle MusStartEffect2(int number, int volume, int pan , int restartflag, int priority)
{
   int i;
   channel_t *cp;
   musHandle handle;
	fx_header_t *header;

	if (libmus_fxheader_single)
	{
		header = libmus_fxheader_single;
		libmus_fxheader_single = NULL;
	}
	else
	{
		header = libmus_fxheader_current;
   	/* are effects present? */
   	if (!header)
   	{
#ifdef _AUDIODEBUG
      	osSyncPrintf("PLAYER_API.C: MusStartEffect2() no effect bank present.\n");
#endif
      	mus_init_bank = NULL;
      	return (0);
   	}
	}

   /* set FX default sample bank */
   if (!mus_init_bank)
      mus_init_bank = header->ptr_addr;
   /* overwrite same effect if it exists */
   if (restartflag)
   {
      for(i=MAX_SONGS, cp=mus_channels2; i<max_channels; i++, cp++)
      {
         if (cp->fx_number==number && cp->fx_addr==header)
         {
         	/* get default priority if required */
            if (priority==-1)
               priority = header->effects[number].priority;
            handle = __MusIntStartEffect(cp, header, number, volume, pan, priority);
            /* reset any single sample bank override */
            mus_init_bank = NULL;
            return (handle);
         }
      }
   }
   /* start effect using parameters */
   handle = __MusIntFindChannelAndStart(header, number, volume, pan, priority);
   /* reset any single sample bank override */
   mus_init_bank = NULL;
   return (handle);
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStop(flags, speed)

  [Parameters]
  flags        bit pattern indicating which types to stop
  speed        number of video frames to stop in

  [Explanation]
  Stop all the specified sound types in the specified number of video frames.

  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusStop(unsigned long flags, int speed)
{
   int i, speed2;
   channel_t *cp;

#ifdef _AUDIODEBUG
   if (speed<0)
   {
      osSyncPrintf("PLAYER_API.C: MusStop() called with invalid speed setting.\n");
      speed = 0;
   }
#endif
   speed2 = speed ? speed : 1;
   for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
   {
      if ((cp->fx_addr && flags&MUSFLAG_EFFECTS) || (!cp->fx_addr && flags&MUSFLAG_SONGS))
      {
         if (cp->pdata && cp->stopping==-1)
         {
            if (cp->channel_flag&CHFLAG_PAUSE)
            {
               cp->stopping_speed = 1;
               cp->stopping = 0;
               cp->channel_flag &= ~CHFLAG_PAUSE;
            }
            else
            {
               cp->stopping_speed = speed2;
               cp->stopping = speed;
            }
         }
      }
   }
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusAsk(flags)

  [Parameters]
  flags        bit pattern indicating which types to test

  [Explanation]
  Count the number of channels that are currently playing.

  [Return value]
  number of channels currently playing
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusAsk(unsigned long flags)
{
  int i, count;
  channel_t *cp;

  for (i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->pdata)
    {
      if ((cp->fx_addr && flags&MUSFLAG_EFFECTS) || (!cp->fx_addr && flags&MUSFLAG_SONGS))
	count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleStop(handle, speed)

  [Parameters]
  handle       sound handle
  speed        number of video frames to stop in

  [Explanation]
  Stop the channels associated with the sound handle in a specified number of video 
  frames.

  [Return value]
  number of channels set to stop
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleStop(musHandle handle, int speed)
{
  int i,speed2, count;
  channel_t *cp;

  if (!handle)
    return(0);

#ifdef _AUDIODEBUG
  if (speed<0)
  {
    osSyncPrintf("PLAYER_API.C: MusHandleStop() called with invalid speed setting.\n");
    speed = 0;
  }
#endif
  speed2 = speed ? speed : 1;
  for (i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle && cp->stopping==-1)
    {
      if (cp->channel_flag&CHFLAG_PAUSE)
      {
	cp->stopping_speed = 1;
	cp->stopping = 0;
	cp->channel_flag &= ~CHFLAG_PAUSE;
      }
      else
      {
	cp->stopping_speed = speed2;
	cp->stopping = speed;
      }
      count++;
    }	
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleAsk(handle)

  [Parameters]
  handle       sound handle

  [Explanation]
  Count the number of channels associated with the specified sound handle that are 
  currently playing.

  [Return value]
  number of channels currently playing
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleAsk(musHandle handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);
  
  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
    if (cp->handle==handle)
      count++;
  return(count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetVolume(handle, volume)

  [Parameters]
  handle       sound handle
  volume       volume scale

  [Explanation]
  Set the volume scale level of the channels associated with the specified sound
  handle. This value scales the channels volume level (from the sound data) in the 
  following mannor:
          volume = (volume*volumescale)>>7

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetVolume(musHandle handle, int volume)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->volscale = volume;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetPan(handle, pan)

  [Parameters]
  handle       sound effect handle
  pan          pan scale

  [Explanation]
  Set the pan scale of the channels associated with the specified sound handle. This
  value scales the channels pan value (from the sound data) in the following mannor:
          pan = (pan*panscale)>>7

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetPan (musHandle handle, int pan)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->panscale = pan;
      cp->old_pan = 0xff;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetFreqOffset(handle, offset)

  [Parameters]
  handle       sound handle
  offset       frequency offset

  [Explanation]
  Set the frequency offset value of the channels associated with the specified sound 
  handle. This value is added to the frequency that is to be output.

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetFreqOffset(musHandle handle, float offset)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->freqoffset = offset+cp->distort;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetTempo(handle, tempo)

  [Parameters]
  handle       sound effect handle
  tempo        tempo scale

  [Explanation]
  Set the tempo scale value of the channels associated with the specified sound 
  handle. This value scales the channels tempo value (from the sound data) in 
  the following mannor:
          tempo = (tempo*temposcale)>>7

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetTempo(musHandle handle, int tempo)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  if (tempo<1)
    tempo=1;
  else if (tempo>256)
    tempo=256;

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->temscale = tempo;
      cp->channel_tempo = (cp->channel_tempo_save*tempo)>>7;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetReverb(handle, reverb)

  [Parameters]
  handle       sound effect handle
  reverb       reverb offset

  [Explanation]
  Set the reverb offset value of the channels associated with the specified sound
  handle. This value is used as a base level with the levels defined in the sound
  data being scaled (with 128-base level) and added to the offset. This can be shown as
  follows:
          channelreverb = reverb+(((128-reverb)*datareverb)>>7)

  NOTE: the change in the base level will only become apparent when a new note is
  fetched on the channels associated with the sound handle
  
  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetReverb(musHandle handle, int reverb)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  if (reverb<0)
    reverb=0;
  else if (reverb>127)
    reverb=127;

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->reverb_base = reverb;
      cp->old_reverb = 0xff;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusPtrBankInitialise(pbank, wbank)

  [Parameters]
  pbank		address of pointer bank
  wbank		address of wave bank

  [Explanation]
  Initialise a sample bank. This function remaps various pointers within the sample 
  pointer bank referencing both itself and the sample wave bank.

  NOTE: The address of the sample pointer bank must be 32-bit aligned although it is
  recommended it be cache aligned.
  
  [Return value]
  none
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusPtrBankInitialize(void *pbank, void *wbank)
{
  __MusIntRemapPtrBank(pbank, wbank);
  if (!mus_default_bank)
	mus_default_bank = pbank;
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusPtrBankSetSingle(ipbank)

  [Parameters]
  ipbank       address of an initialised sample bank

  [Explanation]
  This function will set a sample bank which will be used for the next song or sound
  effect started.

  [Return value]
  sample bank set for next song or sound effect
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void *MusPtrBankSetSingle(void *ipbank)
{
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
#ifdef _AUDIODEBUG
    else
      osSyncPrintf("PLAYER_API.C: MusPtrBankSetSingle() called with invalid pointer bank.\n");
#endif
  }
  return (mus_init_bank);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusPtrBankSetCurrent(ipbank)

  [Parameters]
  ipbank       address of an initialised sample bank

  [Explanation]
  This function will set the default sample bank for all sound effects and songs 
  subsequently started.

  [Return value]
  current default sample bank
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusPtrBankSetCurrent(void *ipbank)
{
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_default_bank = pptr;    
#ifdef _AUDIODEBUG
    else
      osSyncPrintf("PLAYER_API.C: MusPtrBankSetCurrent() called with invalid pointer bank.\n");
#endif
  }
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusPtrBankGetCurrent()

  [Explanation]
  Get the current default sample bank which is used for all sound effects and songs 
  triggered.

  [Return value]
  current default sample bank
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void *MusPtrBankGetCurrent(void)
{
   return (mus_default_bank);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleGetPtrBank(handle)

  [Parameters]
  handle       song handle

  [Explanation]
  Return the address of the sample pointer bank the first channel related to the 
  specified handle uses.
  
  [Return value]
  address of sample pointer bank or NULL if handle is not found
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void *MusHandleGetPtrBank(musHandle handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (NULL);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
    if (cp->handle==handle)
      return (cp->sample_bank);
  return (NULL);
}


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandlePause(handle)

  [Parameters]
  handle       song handle

  [Explanation]
  Pause the processing of any channels associated with the specified handle.
  
  [Return value]
  0 is the pause attempt failed (wait a VSYNC), <>0 pause attempt was processed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandlePause(musHandle handle)
{
  fifo_t fifo_command;

  fifo_command.command = FIFOCMD_PAUSE;
  fifo_command.data = handle;

  return (__MusIntFifoAddCommand(&fifo_command));
}


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleUnPause(handle)

  [Parameters]
  handle       song handle

  [Explanation]
  Unpause the processing of any channels associated with the specified handle.
  
  [Return value]
  0 is the unpause attempt failed (wait a VSYNC), <>0 unpause attempt was processed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleUnPause(musHandle handle)
{
  fifo_t fifo_command;

  fifo_command.command = FIFOCMD_UNPAUSE;
  fifo_command.data = handle;

  return (__MusIntFifoAddCommand(&fifo_command));
}


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusSetFxType(fxtype)

  [Parameters]
  fxtype       audio library FX type

  [Explanation]
  Changes the current configuration of the audio library to support the specified
  FX type.
  
  [Return value]
  0 is the change attempt failed (wait a VSYNC), <>0 change attempt was processed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusSetFxType(int fxtype)
{
  fifo_t fifo_command;

  fifo_command.command = FIFOCMD_CHANGEFX;
  fifo_command.data = fxtype;

  mus_last_fxtype = fxtype;

  return (__MusIntFifoAddCommand(&fifo_command));
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusSetFxType(onoff)

  [Parameters]
  onoff		flag (MUSBOOL_ON or MUSBOOL_OFF)

  [Explanation]
  Set the flag which allows songs to change the audio library FX type. If this flag
  is set to ON songs will be able to change the global audio library FX type. If this
  flag is set to off songs will not be allowed to change the global audio library FX
  type and the last one set by the programmer (or AL_FX_BIGROOM if none has been set)
  will be restored.
  
  [Return value]
  0 is the change attempt failed (wait a VSYNC), <>0 change attempt was processed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusSetSongFxChange(musBool onoff)
{
  int changed;

  changed = 1;
  if (onoff==MUSBOOL_OFF)
    changed = MusSetFxType(mus_last_fxtype);
  if (changed)
    mus_songfxchange_flag = onoff;
  return (changed);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankInitialise(addr)

  [Parameters]
  fxbank      address of sound effect binary file

  [Explanation]
  Initialise a sound effect bank. If no default sound effect bank is currently set,
  then this becomes the default. This function initialises data inside the sound 
  effect bank.

  NOTE: The address of the sound effect bank must be 32-bit aligned.
  
  [Return value]
  none
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

#define OFFSETTOPOINTER(base,offset)	((u32)(base)+(u32)(offset))

void MusFxBankInitialize(void *fxbank)
{
	int i;
	fx_header_t *header;

   header = (fx_header_t *)fxbank;
   if (header->flags&FXFLAG_INITIALISED)
   {
#ifdef _AUDIODEBUG
      osSyncPrintf("PLAYER_API.C : MusFxBankInitialize() called with FX bank already initialized.\n");
#endif
      return;
   }
   if (!libmus_fxheader_current)
      libmus_fxheader_current = header;

   /* clear fx bank flags */
   header->flags = FXFLAG_INITIALISED;
   /* reset sample bank so it uses the default */
   header->ptr_addr = NULL;

	/* remap offsets to pointers */
	header->wave_table = (unsigned short *)OFFSETTOPOINTER(fxbank, header->wave_table);
	for (i=0; i<header->number_of_components; i++)
		header->effects[i].fxdata = (unsigned char *)OFFSETTOPOINTER(fxbank, header->effects[i].fxdata);

}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankNumberOfEffects(ifxbank)

  [Parameters]
  ifxbank      address of sound effect binary file

  [Explanation]
  Return the number of sound effects contained in the sound effect bank at the 
  specified address.
  
  [Return value]
  Number of sound effects
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusFxBankNumberOfEffects(void *ifxbank)
{
   return (((fx_header_t *)ifxbank)->number_of_effects);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankSetCurrent(ifxbank)

  [Parameters]
  ifxbank      address of sound effect binary file

  [Explanation]
  Set the current sound effect bank address. The sound effect bank at the specified
  address must have been previously initialised with MusFxBankInitialize().

  NOTE: The address of the sound effect bank must be 32-bit aligned.

  [Return value]
  none
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusFxBankSetCurrent(void *ifxbank)
{
   libmus_fxheader_current = (fx_header_t *)ifxbank;
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankSetSingle(ifxbank)

  [Parameters]
  ifxbank      address of sound effect binary file

  [Explanation]
  Set a single sound effect bank override.
  
  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusFxBankSetSingle(void *ifxbank)
{
   libmus_fxheader_single = (fx_header_t *)ifxbank;
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankGetCurrent()

  [Explanation]
  Get the address of the current sound effect bank.

  [Return value]
  address of default sound effect bank
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void *MusFxBankGetCurrent(void)
{
   return (libmus_fxheader_current);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankSetPtrBank(ifxbank)

  [Parameters]
  ifxbank       address of sound effect binary file
  ipbank       address of initialised sample bank

  [Explanation]
  Set the default sample bank for the sound effect bank at the specified address.
  The sound effect bank must have been previously initialised with 
  MusFxBankInitialize(). The sample bank must have been previously initialised
  with MusPtrBankInitialize().

  [Return value]
  none
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusFxBankSetPtrBank(void *ifxbank, void *ipbank)
{
   ((fx_header_t *)ifxbank)->ptr_addr = (ptr_bank_t *)ipbank;
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusFxBankGetPtrBank(ifxbank)

  [Parameters]
  ifxbank       address of sound effect binary file

  [Explanation]
  Get the default sample bank for the sound effect bank at the specified address.

  [Return value]
  Address of sample bank
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void *MusFxBankGetPtrBank(void *ifxbank)
{
   return (((fx_header_t *)ifxbank)->ptr_addr);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusSetScheduler(sched_list)

  [Parameters]
  sched_list   address of scheduler function list

  [Explanation]
  Set the scheduler support function callback list. This function must be called
  before the library is initialised to take effect. 

  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusSetScheduler(musSched *sched_list)
{
   __libmus_current_sched = sched_list;
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleWaveCount(handle)

  [Parameters]
  handle			sound handle

  [Explanation]
  Return the number of waves available in the wave table associated with the
  specified sound handle.
  
  [Return value]
  number of entries in the song wave lookup table
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleWaveCount(musHandle handle)
{
   channel_t *cp;
   int i;

   if (!handle)
      return (0);
   for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
   {
      if (cp->handle==handle)
      {
         if (cp->song_addr)
            return (cp->song_addr->num_waves);
			else
				return (cp->fx_addr->num_waves);
      }
   }
   return (0);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleWaveAddress(handle)

  [Parameters]
  handle			sound handle

  [Explanation]
  Return the address of the wave lookup table associated with the specified handle. 
  
  [Return value]
  address of the song wave lookup table
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned short *MusHandleWaveAddress(musHandle handle)
{
   channel_t *cp;
   int i;

   if (!handle)
      return (NULL);

   for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
   {
      if (cp->handle==handle)
      {
         if (cp->song_addr)
            return (cp->song_addr->wave_table);
			else
				return (cp->fx_addr->wave_table);
      }
   }
   return (NULL);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusSetMarkerCallback(callback)

  [Parameters]
  callback		address of marker callback function

  [Explanation]
  Set the marker callback function address. The callback function is called when
  a marker is encountered during the playback of a song.

  The callback function should match the type defined by the 'LIBMUScb_marker'
  structure in the "libmus.h" header file. The handle of the song and the marker
  number are passed back to the callback function. This function is called from
  the audio thread and so must execute as quickly as possible.

  To disable the marker callback feature call this function with NULL as the 
  address callback function.

  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusSetMarkerCallback(void *callback)
{
	marker_callback = (LIBMUScb_marker)callback;
}


/* end of file */
