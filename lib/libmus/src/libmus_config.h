
/*********************************************************

  libmus_config.h : Nintendo 64 Music Tools Programmers Library
  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.

  Version 3.14

  Configuration file for libary compilation this should be
  included first and be the main dependancy for library
  source files. This file should also contain information
  relating to specific library extensions and current
  development work.

  This file should now only be changed during development,
  all configurable flags have been moved to the makefile.

**********************************************************/

#ifndef _LIBMUS_CONFIG_H_
#define _LIBMUS_CONFIG_H_

/* 
	Audio configuration settings moved to makefile:

  _AUDIODEBUG
   Includes limited validation code into the library. Reports
   various types of incorrect data or parameters via the 
   osSyncPrintf() function.

  SUPPORT_NAUDIO
   Configure library to use the "n_audio" library and microcode
   included with the Nintendo LIB2.0i library release.

*/


#define SUPPORT_FXCHANGE
/*
  Development code to support the changing of the audio FX
  bus currently being used. 
  This setting should be set by default.
 */



/* NOTE:

   The following defines control various functions that may
   be included in future versions of the library. These flags
   should not be used in a standard version of the library.
*/

/*#define SUPPORT_PROFILER*/
/*
   This enables CPU performance monitoring for the main driver
   code. The variables "_mus_cpu_last" and "_mus_cpu_worst"
   become available as externals. These variables are calculated 
   using the osGetCount() function.
*/
    
/*#define SUPPORT_EFFECTS*/
/*
  Currently a Software Creations only extension to the library
  which allows the programmer to apply specific effects to the
  continuous volume and pitch bend data.
  This setting should not be used.
*/

/*#define SUPPORT_WAVEMONITORING*/
/*
  Currently a Software Creations only extension which counts
  the number of times a particular sample is used (8-bit only).
*/




#endif /* _LIBMUS_CONFIG_H_ */

/* end of file */
