#include "common.h"
#include "gbi.h"
#include "generated/model_gfx.h"
#include "graphics/displaylist.h"

DisplayLists gPostboxDisplayLists = { 0x00000000, POSTBOX_DISPLAY_LIST_part_01F8, NULL, NULL };

DisplayLists gPostboxWithNewspaperDisplayLists = { 0x00000000,
                                                   POSTBOX_WITH_NEWSPAPER_DISPLAY_LIST_part_0220,
                                                   NULL,
                                                   NULL };

DisplayLists D_800886D0[1] = {
    { 0x00000000, CAKE_DISPLAY_LIST_part_0368, CAKE_DISPLAY_LIST_part_0378, NULL }
};

DisplayLists gCrabDisplayLists = { 0x00000000, CRAB_DISPLAY_LIST_part_0280, CRAB_DISPLAY_LIST_part_0290, NULL };

DisplayLists D_800886F0[1] = {
    { 0x00000000, NULL, CAKEE_DISPLAY_LIST_part_01F0, NULL }
};

DisplayLists gChestClosedDisplayLists = { 0x00000000, CHEST_CLOSED_DISPLAY_LIST_part_0270, NULL, NULL };

DisplayLists gChestOpenDisplayLists = { 0x00000000, CHEST_OPEN_DISPLAY_LIST_part_02D8, NULL, NULL };
