#ifndef _199C0_H_
#define _199C0_H_

#include "common.h"

extern u8 storyMapLocationIndex;
extern void (*storyMapLocationHandlers[])(void);
extern s16 storyMapExitCode;

void initStoryMapLocation(void);

#endif
