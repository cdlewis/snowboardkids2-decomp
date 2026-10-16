#include "data/model_display_lists.h"
#include "generated/model_gfx.h"

DisplayLists gTable3DisplayLists = { 0x00000000, TABLE3_DISPLAY_LIST_part_0180, NULL, NULL };

DisplayLists gTable2DisplayLists = { 0x00000000, TABLE2_DISPLAY_LIST_part_03B0, TABLE2_DISPLAY_LIST_part_03C0, NULL };

DisplayLists gLindaBDisplayLists = { 0x00000000, LINDA_B_DISPLAY_LIST_part_0480, LINDA_B_DISPLAY_LIST_part_0490, NULL };

DisplayLists gTable1DisplayLists = { 0x00000000, TABLE1_DISPLAY_LIST_part_0180, NULL, NULL };

DisplayLists gTable0DisplayLists = { 0x00000000, TABLE0_DISPLAY_LIST_part_02E0, TABLE0_DISPLAY_LIST_part_02F0, NULL };

DisplayLists gLindaPDisplayLists = { 0x00000000, LINDA_P_DISPLAY_LIST_part_0488, LINDA_P_DISPLAY_LIST_part_0498, NULL };

DisplayLists gHauntedHouseDoorDisplayLists = { 0x00000000,
                                               HAUNTED_HOUSE_DOOR_DISPLAY_LIST_part_05A0,
                                               HAUNTED_HOUSE_DOOR_DISPLAY_LIST_part_05B0,
                                               NULL };

DisplayLists gBomb2DisplayLists = { 0x00000000, BOMB2_DISPLAY_LIST_part_0268, NULL, NULL };

DisplayLists gOtherHauntedPrimaryDisplayLists = { 0x00000000,
                                                  OTHER_HAUNTED_DISPLAY_LIST_part_0560,
                                                  OTHER_HAUNTED_DISPLAY_LIST_part_0570,
                                                  NULL };

DisplayLists gOtherHauntedSecondaryPartDisplayLists[2] = {
    { 0x00000000, OTHER_HAUNTED_DISPLAY_LIST_part_07C0, NULL, NULL },
    { 0x00000000, OTHER_HAUNTED_DISPLAY_LIST_part_0838, NULL, NULL }
};

DisplayLists gHauntedDisplayLists = { 0x00000000, HAUNTED_DISPLAY_LIST_part_0210, NULL, NULL };

DisplayLists gMroboPartDisplayLists[10] = {
    { 0x00000000, MROBO_DISPLAY_LIST_part_0000, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_02A8, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_04F8, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_08B8, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_0B10, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_0ED0, MROBO_DISPLAY_LIST_part_1320, NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_1408, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_1540, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_1728, NULL,                         NULL },
    { 0x00000000, MROBO_DISPLAY_LIST_part_1860, NULL,                         NULL }
};

DisplayLists gHamburgerDisplayLists = { 0x00000000, HAMBURGER_DISPLAY_LIST_part_0180, NULL, NULL };

DisplayLists gHelicopterDisplayLists = { 0x00000000, HELICOPTER_DISPLAY_LIST_part_0760, NULL, NULL };

DisplayLists gBobbingModelOverlayDisplayLists = { 0x00000000, NULL, HELICOPTER_DISPLAY_LIST_part_0888, NULL };

DisplayLists gBunnyDisplayLists = { 0x00000000, BUNNY_DISPLAY_LIST_part_0270, NULL, NULL };

DisplayLists gPresentRedDisplayLists = { 0x00000000, PRESENT_RED_DISPLAY_LIST_part_0140, NULL, NULL };

DisplayLists gPresentBlueDisplayLists = { 0x00000000, PRESENT_BLUE_DISPLAY_LIST_part_0140, NULL, NULL };

DisplayLists gItem1DisplayLists = { 0x00000000, ITEM_1_DISPLAY_LIST_part_0140, NULL, NULL };

DisplayLists gItemDebrisDisplayLists = { 0x00000000, ITEM_DEBRIS_DISPLAY_LIST_part_0140, NULL, NULL };

DisplayLists gDishDisplayLists = { 0x00000000, DISH_DISPLAY_LIST_part_01A8, NULL, NULL };

DisplayLists gTutumDisplayLists = { 0x00000000, TUTUM_DISPLAY_LIST_part_01E0, NULL, NULL };

DisplayLists gTableDisplayLists = { 0x00000000, TABLE_DISPLAY_LIST_part_0238, TABLE_DISPLAY_LIST_part_0248, NULL };

DisplayLists gRadioDisplayLists = { 0x00000000, RADIO_DISPLAY_LIST_part_02A8, RADIO_DISPLAY_LIST_part_02B8, NULL };

DisplayLists gPotDisplayLists = { 0x00000000, POT_DISPLAY_LIST_part_0398, POT_DISPLAY_LIST_part_03A8, NULL };

DisplayLists gCupDisplayLists = { 0x00000000, CUP_DISPLAY_LIST_part_0418, CUP_DISPLAY_LIST_part_0428, NULL };

DisplayLists gChairPartDisplayLists[3] = {
    { 0x00000000, CHAIR_DISPLAY_LIST_part_01C8, NULL, NULL                          },
    { 0x00000000, NULL,                         NULL, UNUSED_DISPLAY_LIST_part_0000 },
    { 0x00000000, NULL,                         NULL, UNUSED_DISPLAY_LIST_part_01E8 }
};

DisplayLists gSnow3PartDisplayLists[3] = {
    { 0x00000000, NULL, NULL, SNOW3_DISPLAY_LIST_part_0000 },
    { 0x00000000, NULL, NULL, SNOW3_DISPLAY_LIST_part_01E0 },
    { 0x00000000, NULL, NULL, SNOW3_DISPLAY_LIST_part_03C0 }
};

DisplayLists gSnow2PartDisplayLists[2] = {
    { 0x00000000, SNOW2_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, SNOW2_DISPLAY_LIST_part_01E8, NULL, NULL }
};

DisplayLists gTownPrimaryDisplayLists = { 0x00000000, TOWN_DISPLAY_LIST_part_1A40, TOWN_DISPLAY_LIST_part_1A50, NULL };

DisplayLists gTownSwingingModel0DisplayLists = { 0x00000000, NULL, TOWN_DISPLAY_LIST_part_1B40, NULL };

DisplayLists gTownRotatingModel0DisplayLists = { 0x00000000, NULL, TOWN_DISPLAY_LIST_part_1CA0, NULL };

DisplayLists gTownSwingingModel1DisplayLists = { 0x00000000, NULL, TOWN_DISPLAY_LIST_part_1D90, NULL };

DisplayLists gTownSwingingModel2DisplayLists = { 0x00000000, NULL, TOWN_DISPLAY_LIST_part_1E80, NULL };

DisplayLists gTownSwingingModel3DisplayLists = { 0x00000000, NULL, TOWN_DISPLAY_LIST_part_1F70, NULL };

DisplayLists gTownSecondaryPartDisplayLists[2] = {
    { 0x00000000, TOWN_DISPLAY_LIST_part_2248,           NULL,                                  NULL },
    { 0x00000000, TOWN_ALTERNATE_DISPLAY_LIST_part_20D8, TOWN_ALTERNATE_DISPLAY_LIST_part_2130, NULL }
};

DisplayLists gTownSwingingModel4DisplayLists = { 0x00000000, NULL, TOWN_ALTERNATE_DISPLAY_LIST_part_2268, NULL };

DisplayLists gTownRotatingModel1DisplayLists = { 0x00000000, NULL, TOWN_ALTERNATE_DISPLAY_LIST_part_23C8, NULL };

DisplayLists gTownSwingingModel5DisplayLists = { 0x00000000, NULL, TOWN_ALTERNATE_DISPLAY_LIST_part_24B8, NULL };

DisplayLists gTownSwingingModel6DisplayLists = { 0x00000000, NULL, TOWN_ALTERNATE_DISPLAY_LIST_part_25A8, NULL };

DisplayLists gTownSwingingModel7PartDisplayLists[2] = {
    { 0x00000000, NULL,                                  TOWN_ALTERNATE_DISPLAY_LIST_part_2698, NULL },
    { 0x00000000, TOWN_ALTERNATE_DISPLAY_LIST_part_2968, NULL,                                  NULL }
};

DisplayLists gJunglePrimaryDisplayLists = { 0x00000000,
                                            JUNGLE_DISPLAY_LIST_part_0658,
                                            JUNGLE_DISPLAY_LIST_part_0668,
                                            NULL };

DisplayLists gJungleSecondaryDisplayLists = { 0x00000000, JUNGLE_DISPLAY_LIST_part_08F8, NULL, NULL };

DisplayLists gJungleAnimatedModel0DisplayLists = { 0x00000000, JUNGLE_DISPLAY_LIST_part_0978, NULL, NULL };

DisplayLists gJungleAnimatedModel1DisplayLists = { 0x00000000, JUNGLE_DISPLAY_LIST_part_0A10, NULL, NULL };

DisplayLists gJungleStaticModelDisplayLists = { 0x00000000, NULL, NULL, JUNGLE_DISPLAY_LIST_part_0B30 };

DisplayLists gJungle2PrimaryDisplayLists = { 0x00000000,
                                             JUNGLE2_DISPLAY_LIST_part_0890,
                                             JUNGLE2_DISPLAY_LIST_part_08A0,
                                             NULL };

DisplayLists gJungle2SecondaryDisplayLists = { 0x00000000, JUNGLE2_DISPLAY_LIST_part_0C38, NULL, NULL };

DisplayLists gJungle2AnimatedModel0DisplayLists = { 0x00000000, JUNGLE2_DISPLAY_LIST_part_0CB8, NULL, NULL };

DisplayLists gJungle2AnimatedModel1DisplayLists = { 0x00000000, JUNGLE2_DISPLAY_LIST_part_0D50, NULL, NULL };

DisplayLists gSpacePrimaryDisplayLists = { 0x00000000,
                                           SPACE_DISPLAY_LIST_part_0910,
                                           SPACE_DISPLAY_LIST_part_0920,
                                           NULL };

DisplayLists gSpaceSecondaryDisplayLists = { 0x00000000, SPACE_DISPLAY_LIST_part_0D18, NULL, NULL };

DisplayLists gYamaPrimaryDisplayLists = { 0x00000000, YAMA_DISPLAY_LIST_part_0DD0, YAMA_DISPLAY_LIST_part_0DE0, NULL };

DisplayLists gYamaSecondaryDisplayLists = { 0x00000000, YAMA_DISPLAY_LIST_part_10A0, NULL, NULL };

DisplayLists gYamaYPrimaryDisplayLists = { 0x00000000,
                                           YAMA_Y_DISPLAY_LIST_part_0D10,
                                           YAMA_Y_DISPLAY_LIST_part_0D20,
                                           NULL };

DisplayLists gYamaYSecondaryDisplayLists = { 0x00000000, YAMA_Y_DISPLAY_LIST_part_0F50, NULL, NULL };

DisplayLists gUfoDisplayLists = { DISPLAY_LIST_FLAG_REFLECTION_LOOK_AT, UFO_DISPLAY_LIST_part_04E8, NULL, NULL };
