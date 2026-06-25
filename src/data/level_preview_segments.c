#include "common.h"
#include "graphics/displaylist.h"

DisplayLists gPostboxDisplayLists = { 0x00000000, (Gfx *)0x010001F8, NULL, NULL };

DisplayLists gPostboxWithNewspaperDisplayLists = { 0x00000000, (Gfx *)0x01000220, NULL, NULL };

u32 D_800886D0[] = { 0x00000000, 0x01000368, 0x01000378, 0x00000000 };

DisplayLists gCrabDisplayLists = { 0x00000000, (Gfx *)0x01000280, (Gfx *)0x01000290, NULL };

u32 D_800886F0[] = { 0x00000000, 0x00000000, 0x010001F0, 0x00000000 };

DisplayLists gChestClosedDisplayLists = { 0x00000000, (Gfx *)0x01000270, NULL, NULL };

DisplayLists gChestOpenDisplayLists = { 0x00000000, (Gfx *)0x010002D8, NULL, NULL };
