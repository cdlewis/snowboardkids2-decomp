#include "data/asset_metadata.h"
#include "assets.h"
#include "common.h"
#include "gamestate.h"
#include "gbi.h"
#include "generated/model_gfx.h"
#include "graphics/displaylist.h"
#include "race/race_session.h"
#include "system/memory_allocator.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

DisplayLists gSlashBoard0PrimaryPartDisplayLists[];
DisplayLists gSlashBoard0SecondaryPartDisplayLists[];
DisplayLists gSlashBoard1PrimaryPartDisplayLists[];
DisplayLists gSlashBoard1SecondaryPartDisplayLists[];
DisplayLists gSlashBoard2PrimaryPartDisplayLists[];
DisplayLists gSlashBoard2SecondaryPartDisplayLists[];
DisplayLists gSlashBoard3PrimaryPartDisplayLists[];
DisplayLists gSlashBoard3SecondaryPartDisplayLists[];
DisplayLists gNancyBoard0PrimaryPartDisplayLists[];
DisplayLists gNancyBoard0SecondaryPartDisplayLists[];
DisplayLists gNancyBoard1PrimaryPartDisplayLists[];
DisplayLists gNancyBoard1SecondaryPartDisplayLists[];
DisplayLists gNancyBoard2PrimaryPartDisplayLists[];
DisplayLists gNancyBoard2SecondaryPartDisplayLists[];
DisplayLists gNancyBoard3PrimaryPartDisplayLists[];
DisplayLists gNancyBoard3SecondaryPartDisplayLists[];
DisplayLists gJamBoard0PrimaryPartDisplayLists[];
DisplayLists gJamBoard0SecondaryPartDisplayLists[];
DisplayLists gJamBoard1PrimaryPartDisplayLists[];
DisplayLists gJamBoard1SecondaryPartDisplayLists[];
DisplayLists gJamBoard2PrimaryPartDisplayLists[];
DisplayLists gJamBoard2SecondaryPartDisplayLists[];
DisplayLists gJamBoard3PrimaryPartDisplayLists[];
DisplayLists gJamBoard3SecondaryPartDisplayLists[];
DisplayLists gLindaBoard0PrimaryPartDisplayLists[];
DisplayLists gLindaBoard0SecondaryPartDisplayLists[];
DisplayLists gLindaBoard1PrimaryPartDisplayLists[];
DisplayLists gLindaBoard1SecondaryPartDisplayLists[];
DisplayLists gLindaBoard2PrimaryPartDisplayLists[];
DisplayLists gLindaBoard2SecondaryPartDisplayLists[];
DisplayLists gLindaBoard3PrimaryPartDisplayLists[];
DisplayLists gLindaBoard3SecondaryPartDisplayLists[];
DisplayLists gTommyBoard0PrimaryPartDisplayLists[];
DisplayLists gTommyBoard0SecondaryPartDisplayLists[];
DisplayLists gTommyBoard1PrimaryPartDisplayLists[];
DisplayLists gTommyBoard1SecondaryPartDisplayLists[];
DisplayLists gTommyBoard2PrimaryPartDisplayLists[];
DisplayLists gTommyBoard2SecondaryPartDisplayLists[];
DisplayLists gTommyBoard3PrimaryPartDisplayLists[];
DisplayLists gTommyBoard3SecondaryPartDisplayLists[];
DisplayLists gWendyBoard0PrimaryPartDisplayLists[];
DisplayLists gWendyBoard0SecondaryPartDisplayLists[];
DisplayLists gWendyBoard1PrimaryPartDisplayLists[];
DisplayLists gWendyBoard1SecondaryPartDisplayLists[];
DisplayLists gWendyBoard2PrimaryPartDisplayLists[];
DisplayLists gWendyBoard2SecondaryPartDisplayLists[];
DisplayLists gWendyBoard3PrimaryPartDisplayLists[];
DisplayLists gWendyBoard3SecondaryPartDisplayLists[];
DisplayLists gDamienBoard0PrimaryPartDisplayLists[];
DisplayLists gDamienBoard0SecondaryPartDisplayLists[];
DisplayLists gDamienBoard1PrimaryPartDisplayLists[];
DisplayLists gDamienBoard1SecondaryPartDisplayLists[];
DisplayLists gDamienBoard2PrimaryPartDisplayLists[];
DisplayLists gDamienBoard2SecondaryPartDisplayLists[];
DisplayLists gDamienBoard3PrimaryPartDisplayLists[];
DisplayLists gDamienBoard3SecondaryPartDisplayLists[];
DisplayLists gPenBoard0PrimaryPartDisplayLists[];
DisplayLists gPenBoard0SecondaryPartDisplayLists[];
DisplayLists gPenBoard1PrimaryPartDisplayLists[];
DisplayLists gPenBoard1SecondaryPartDisplayLists[];
DisplayLists gPenBoard2PrimaryPartDisplayLists[];
DisplayLists gPenBoard2SecondaryPartDisplayLists[];
DisplayLists gPenBoard3PrimaryPartDisplayLists[];
DisplayLists gPenBoard3SecondaryPartDisplayLists[];
DisplayLists gDogBoard0PrimaryPartDisplayLists[];
DisplayLists gDogBoard0SecondaryPartDisplayLists[];
DisplayLists gDogBoard1PrimaryPartDisplayLists[];
DisplayLists gDogBoard1SecondaryPartDisplayLists[];
DisplayLists gDogBoard2PrimaryPartDisplayLists[];
DisplayLists gDogBoard2SecondaryPartDisplayLists[];
DisplayLists gDogBoard3PrimaryPartDisplayLists[];
DisplayLists gDogBoard3SecondaryPartDisplayLists[];
DisplayLists gDinosBoardPrimaryPartDisplayLists[];
DisplayLists gSnowBoardPrimaryPartDisplayLists[];
DisplayLists gRoboBoardPrimaryPartDisplayLists[];
AssetMeta D_8009A550_9B150[];

AssetMeta gSlashBoardDisplayListAssets[] = {
    { &SLASH_BOARD0_DISPLAY_LIST_ROM_START, &SLASH_BOARD0_DISPLAY_LIST_ROM_END },
    { &SLASH_BOARD1_DISPLAY_LIST_ROM_START, &SLASH_BOARD1_DISPLAY_LIST_ROM_END },
    { &SLASH_BOARD2_DISPLAY_LIST_ROM_START, &SLASH_BOARD2_DISPLAY_LIST_ROM_END },
    { &SLASH_BOARD3_DISPLAY_LIST_ROM_START, &SLASH_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gNancyBoardDisplayListAssets[] = {
    { &NANCY_BOARD0_DISPLAY_LIST_ROM_START, &NANCY_BOARD0_DISPLAY_LIST_ROM_END },
    { &NANCY_BOARD1_DISPLAY_LIST_ROM_START, &NANCY_BOARD1_DISPLAY_LIST_ROM_END },
    { &NANCY_BOARD2_DISPLAY_LIST_ROM_START, &NANCY_BOARD2_DISPLAY_LIST_ROM_END },
    { &NANCY_BOARD3_DISPLAY_LIST_ROM_START, &NANCY_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gJamBoardDisplayListAssets[] = {
    { &JAM_BOARD0_DISPLAY_LIST_ROM_START, &JAM_BOARD0_DISPLAY_LIST_ROM_END },
    { &JAM_BOARD1_DISPLAY_LIST_ROM_START, &JAM_BOARD1_DISPLAY_LIST_ROM_END },
    { &JAM_BOARD2_DISPLAY_LIST_ROM_START, &JAM_BOARD2_DISPLAY_LIST_ROM_END },
    { &JAM_BOARD3_DISPLAY_LIST_ROM_START, &JAM_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gLindaBoardDisplayListAssets[] = {
    { &LINDA_BOARD0_DISPLAY_LIST_ROM_START, &LINDA_BOARD0_DISPLAY_LIST_ROM_END },
    { &LINDA_BOARD1_DISPLAY_LIST_ROM_START, &LINDA_BOARD1_DISPLAY_LIST_ROM_END },
    { &LINDA_BOARD2_DISPLAY_LIST_ROM_START, &LINDA_BOARD2_DISPLAY_LIST_ROM_END },
    { &LINDA_BOARD3_DISPLAY_LIST_ROM_START, &LINDA_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gTommyBoardDisplayListAssets[] = {
    { &TOMMY_BOARD0_DISPLAY_LIST_ROM_START, &TOMMY_BOARD0_DISPLAY_LIST_ROM_END },
    { &TOMMY_BOARD1_DISPLAY_LIST_ROM_START, &TOMMY_BOARD1_DISPLAY_LIST_ROM_END },
    { &TOMMY_BOARD2_DISPLAY_LIST_ROM_START, &TOMMY_BOARD2_DISPLAY_LIST_ROM_END },
    { &TOMMY_BOARD3_DISPLAY_LIST_ROM_START, &TOMMY_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gWendyBoardDisplayListAssets[] = {
    { &WENDY_BOARD0_DISPLAY_LIST_ROM_START, &WENDY_BOARD0_DISPLAY_LIST_ROM_END },
    { &WENDY_BOARD1_DISPLAY_LIST_ROM_START, &WENDY_BOARD1_DISPLAY_LIST_ROM_END },
    { &WENDY_BOARD2_DISPLAY_LIST_ROM_START, &WENDY_BOARD2_DISPLAY_LIST_ROM_END },
    { &WENDY_BOARD3_DISPLAY_LIST_ROM_START, &WENDY_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gDamienBoardDisplayListAssets[] = {
    { &DAMIEN_BOARD0_DISPLAY_LIST_ROM_START, &DAMIEN_BOARD0_DISPLAY_LIST_ROM_END },
    { &DAMIEN_BOARD1_DISPLAY_LIST_ROM_START, &DAMIEN_BOARD1_DISPLAY_LIST_ROM_END },
    { &DAMIEN_BOARD2_DISPLAY_LIST_ROM_START, &DAMIEN_BOARD2_DISPLAY_LIST_ROM_END },
    { &DAMIEN_BOARD3_DISPLAY_LIST_ROM_START, &DAMIEN_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gPenBoardDisplayListAssets[] = {
    { &PEN_BOARD0_DISPLAY_LIST_ROM_START, &PEN_BOARD0_DISPLAY_LIST_ROM_END },
    { &PEN_BOARD1_DISPLAY_LIST_ROM_START, &PEN_BOARD1_DISPLAY_LIST_ROM_END },
    { &PEN_BOARD2_DISPLAY_LIST_ROM_START, &PEN_BOARD2_DISPLAY_LIST_ROM_END },
    { &PEN_BOARD3_DISPLAY_LIST_ROM_START, &PEN_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gDogBoardDisplayListAssets[] = {
    { &DOG_BOARD0_DISPLAY_LIST_ROM_START, &DOG_BOARD0_DISPLAY_LIST_ROM_END },
    { &DOG_BOARD1_DISPLAY_LIST_ROM_START, &DOG_BOARD1_DISPLAY_LIST_ROM_END },
    { &DOG_BOARD2_DISPLAY_LIST_ROM_START, &DOG_BOARD2_DISPLAY_LIST_ROM_END },
    { &DOG_BOARD3_DISPLAY_LIST_ROM_START, &DOG_BOARD3_DISPLAY_LIST_ROM_END },
};

AssetMeta gDinosBoardDisplayListAssets[] = {
    { &DINOS_BOARD_DISPLAY_LIST_ROM_START, &DINOS_BOARD_DISPLAY_LIST_ROM_END },
    { &DINOS_BOARD_DISPLAY_LIST_ROM_START, &DINOS_BOARD_DISPLAY_LIST_ROM_END },
    { &DINOS_BOARD_DISPLAY_LIST_ROM_START, &DINOS_BOARD_DISPLAY_LIST_ROM_END },
    { &DINOS_BOARD_DISPLAY_LIST_ROM_START, &DINOS_BOARD_DISPLAY_LIST_ROM_END },
};

AssetMeta gSnowBoardDisplayListAssets[] = {
    { &SNOW4_DISPLAY_LIST_ROM_START, &SNOW4_DISPLAY_LIST_ROM_END },
    { &SNOW4_DISPLAY_LIST_ROM_START, &SNOW4_DISPLAY_LIST_ROM_END },
    { &SNOW4_DISPLAY_LIST_ROM_START, &SNOW4_DISPLAY_LIST_ROM_END },
    { &SNOW4_DISPLAY_LIST_ROM_START, &SNOW4_DISPLAY_LIST_ROM_END },
};

AssetMeta gRoboBoardDisplayListAssets[] = {
    { &ROBO_BOARD_DISPLAY_LIST_ROM_START, &ROBO_BOARD_DISPLAY_LIST_ROM_END },
    { &ROBO_BOARD_DISPLAY_LIST_ROM_START, &ROBO_BOARD_DISPLAY_LIST_ROM_END },
    { &ROBO_BOARD_DISPLAY_LIST_ROM_START, &ROBO_BOARD_DISPLAY_LIST_ROM_END },
    { &ROBO_BOARD_DISPLAY_LIST_ROM_START, &ROBO_BOARD_DISPLAY_LIST_ROM_END },
};

AssetMeta *gCharacterBoardDisplayListTables[] = {
    gSlashBoardDisplayListAssets,  gNancyBoardDisplayListAssets, gJamBoardDisplayListAssets,
    gLindaBoardDisplayListAssets,  gTommyBoardDisplayListAssets, gWendyBoardDisplayListAssets,
    gDamienBoardDisplayListAssets, gPenBoardDisplayListAssets,   gDogBoardDisplayListAssets,
    gDinosBoardDisplayListAssets,  gSnowBoardDisplayListAssets,  gRoboBoardDisplayListAssets,
};

CompressedAssetMeta gSlashBoardCompressedDataAssets[] = {
    { &SLASH_BOARD0_COMPRESSED_DATA_ROM_START, &SLASH_BOARD0_COMPRESSED_DATA_ROM_END, 0x45A0 },
    { &SLASH_BOARD1_COMPRESSED_DATA_ROM_START, &SLASH_BOARD1_COMPRESSED_DATA_ROM_END, 0x4870 },
    { &SLASH_BOARD2_COMPRESSED_DATA_ROM_START, &SLASH_BOARD2_COMPRESSED_DATA_ROM_END, 0x54D0 },
    { &SLASH_BOARD3_COMPRESSED_DATA_ROM_START, &SLASH_BOARD3_COMPRESSED_DATA_ROM_END, 0x48E0 },
};

CompressedAssetMeta gNancyBoardCompressedDataAssets[] = {
    { &NANCY_BOARD0_COMPRESSED_DATA_ROM_START, &NANCY_BOARD0_COMPRESSED_DATA_ROM_END, 0x42A0 },
    { &NANCY_BOARD1_COMPRESSED_DATA_ROM_START, &NANCY_BOARD1_COMPRESSED_DATA_ROM_END, 0x43F0 },
    { &NANCY_BOARD2_COMPRESSED_DATA_ROM_START, &NANCY_BOARD2_COMPRESSED_DATA_ROM_END, 0x43E0 },
    { &NANCY_BOARD3_COMPRESSED_DATA_ROM_START, &NANCY_BOARD3_COMPRESSED_DATA_ROM_END, 0x4120 },
};

CompressedAssetMeta gJamBoardCompressedDataAssets[] = {
    { &JAM_BOARD0_COMPRESSED_DATA_ROM_START, &JAM_BOARD0_COMPRESSED_DATA_ROM_END, 0x4300 },
    { &JAM_BOARD1_COMPRESSED_DATA_ROM_START, &JAM_BOARD1_COMPRESSED_DATA_ROM_END, 0x4D10 },
    { &JAM_BOARD2_COMPRESSED_DATA_ROM_START, &JAM_BOARD2_COMPRESSED_DATA_ROM_END, 0x4640 },
    { &JAM_BOARD3_COMPRESSED_DATA_ROM_START, &JAM_BOARD3_COMPRESSED_DATA_ROM_END, 0x49C0 },
};

CompressedAssetMeta gLindaBoardCompressedDataAssets[] = {
    { &LINDA_BOARD0_COMPRESSED_DATA_ROM_START, &LINDA_BOARD0_COMPRESSED_DATA_ROM_END, 0x4360 },
    { &LINDA_BOARD1_COMPRESSED_DATA_ROM_START, &LINDA_BOARD1_COMPRESSED_DATA_ROM_END, 0x4160 },
    { &LINDA_BOARD2_COMPRESSED_DATA_ROM_START, &LINDA_BOARD2_COMPRESSED_DATA_ROM_END, 0x45F0 },
    { &LINDA_BOARD3_COMPRESSED_DATA_ROM_START, &LINDA_BOARD3_COMPRESSED_DATA_ROM_END, 0x3E40 },
};

CompressedAssetMeta gTommyBoardCompressedDataAssets[] = {
    { &TOMMY_BOARD0_COMPRESSED_DATA_ROM_START, &TOMMY_BOARD0_COMPRESSED_DATA_ROM_END, 0x5CB0 },
    { &TOMMY_BOARD1_COMPRESSED_DATA_ROM_START, &TOMMY_BOARD1_COMPRESSED_DATA_ROM_END, 0x66D0 },
    { &TOMMY_BOARD2_COMPRESSED_DATA_ROM_START, &TOMMY_BOARD2_COMPRESSED_DATA_ROM_END, 0x6850 },
    { &TOMMY_BOARD3_COMPRESSED_DATA_ROM_START, &TOMMY_BOARD3_COMPRESSED_DATA_ROM_END, 0x5160 },
};

CompressedAssetMeta gWendyBoardCompressedDataAssets[] = {
    { &WENDY_BOARD0_COMPRESSED_DATA_ROM_START, &WENDY_BOARD0_COMPRESSED_DATA_ROM_END, 0x4710 },
    { &WENDY_BOARD1_COMPRESSED_DATA_ROM_START, &WENDY_BOARD1_COMPRESSED_DATA_ROM_END, 0x4F60 },
    { &WENDY_BOARD2_COMPRESSED_DATA_ROM_START, &WENDY_BOARD2_COMPRESSED_DATA_ROM_END, 0x5550 },
    { &WENDY_BOARD3_COMPRESSED_DATA_ROM_START, &WENDY_BOARD3_COMPRESSED_DATA_ROM_END, 0x4C40 },
};

CompressedAssetMeta gDamienBoardCompressedDataAssets[] = {
    { &DAMIEN_BOARD0_COMPRESSED_DATA_ROM_START, &DAMIEN_BOARD0_COMPRESSED_DATA_ROM_END, 0x4EE0 },
    { &DAMIEN_BOARD1_COMPRESSED_DATA_ROM_START, &DAMIEN_BOARD1_COMPRESSED_DATA_ROM_END, 0x5070 },
    { &DAMIEN_BOARD2_COMPRESSED_DATA_ROM_START, &DAMIEN_BOARD2_COMPRESSED_DATA_ROM_END, 0x5670 },
    { &DAMIEN_BOARD3_COMPRESSED_DATA_ROM_START, &DAMIEN_BOARD3_COMPRESSED_DATA_ROM_END, 0x4F60 },
};

CompressedAssetMeta gPenBoardCompressedDataAssets[] = {
    { &PEN_BOARD0_COMPRESSED_DATA_ROM_START, &PEN_BOARD0_COMPRESSED_DATA_ROM_END, 0x4450 },
    { &PEN_BOARD1_COMPRESSED_DATA_ROM_START, &PEN_BOARD1_COMPRESSED_DATA_ROM_END, 0x5350 },
    { &PEN_BOARD2_COMPRESSED_DATA_ROM_START, &PEN_BOARD2_COMPRESSED_DATA_ROM_END, 0x4490 },
    { &PEN_BOARD3_COMPRESSED_DATA_ROM_START, &PEN_BOARD3_COMPRESSED_DATA_ROM_END, 0x4420 },
};

CompressedAssetMeta gDogBoardCompressedDataAssets[] = {
    { &DOG_BOARD0_COMPRESSED_DATA_ROM_START, &DOG_BOARD0_COMPRESSED_DATA_ROM_END, 0x4BE0 },
    { &DOG_BOARD1_COMPRESSED_DATA_ROM_START, &DOG_BOARD1_COMPRESSED_DATA_ROM_END, 0x47C0 },
    { &DOG_BOARD2_COMPRESSED_DATA_ROM_START, &DOG_BOARD2_COMPRESSED_DATA_ROM_END, 0x48A0 },
    { &DOG_BOARD3_COMPRESSED_DATA_ROM_START, &DOG_BOARD3_COMPRESSED_DATA_ROM_END, 0x4DD0 },
};

CompressedAssetMeta gDinosBoardCompressedDataAssets[] = {
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START, &DINOS_BOARD_COMPRESSED_DATA_ROM_END, 0x1D80 },
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START, &DINOS_BOARD_COMPRESSED_DATA_ROM_END, 0x1D80 },
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START, &DINOS_BOARD_COMPRESSED_DATA_ROM_END, 0x1D80 },
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START, &DINOS_BOARD_COMPRESSED_DATA_ROM_END, 0x1D80 },
};

CompressedAssetMeta gSnowBoardCompressedDataAssets[] = {
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, 0xD50 },
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, 0xD50 },
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, 0xD50 },
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, 0xD50 },
};

CompressedAssetMeta gRoboBoardCompressedDataAssets[] = {
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START, &ROBO_BOARD_COMPRESSED_DATA_ROM_END, 0x5280 },
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START, &ROBO_BOARD_COMPRESSED_DATA_ROM_END, 0x5280 },
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START, &ROBO_BOARD_COMPRESSED_DATA_ROM_END, 0x5280 },
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START, &ROBO_BOARD_COMPRESSED_DATA_ROM_END, 0x5280 },
};

CompressedAssetMeta *gCharacterBoardCompressedDataTables[] = {
    gSlashBoardCompressedDataAssets,  gNancyBoardCompressedDataAssets, gJamBoardCompressedDataAssets,
    gLindaBoardCompressedDataAssets,  gTommyBoardCompressedDataAssets, gWendyBoardCompressedDataAssets,
    gDamienBoardCompressedDataAssets, gPenBoardCompressedDataAssets,   gDogBoardCompressedDataAssets,
    gDinosBoardCompressedDataAssets,  gSnowBoardCompressedDataAssets,  gRoboBoardCompressedDataAssets,
};

DisplayLists *gSlashBoardPrimaryPartDisplayListTables[] = {
    gSlashBoard0PrimaryPartDisplayLists,
    gSlashBoard1PrimaryPartDisplayLists,
    gSlashBoard2PrimaryPartDisplayLists,
    gSlashBoard3PrimaryPartDisplayLists,
};

DisplayLists *gSlashBoardSecondaryPartDisplayListTables[] = {
    gSlashBoard0SecondaryPartDisplayLists,
    gSlashBoard1SecondaryPartDisplayLists,
    gSlashBoard2SecondaryPartDisplayLists,
    gSlashBoard3SecondaryPartDisplayLists,
};

DisplayLists *gNancyBoardPrimaryPartDisplayListTables[] = {
    gNancyBoard0PrimaryPartDisplayLists,
    gNancyBoard1PrimaryPartDisplayLists,
    gNancyBoard2PrimaryPartDisplayLists,
    gNancyBoard3PrimaryPartDisplayLists,
};

DisplayLists *gNancyBoardSecondaryPartDisplayListTables[] = {
    gNancyBoard0SecondaryPartDisplayLists,
    gNancyBoard1SecondaryPartDisplayLists,
    gNancyBoard2SecondaryPartDisplayLists,
    gNancyBoard3SecondaryPartDisplayLists,
};

DisplayLists *gJamBoardPrimaryPartDisplayListTables[] = {
    gJamBoard0PrimaryPartDisplayLists,
    gJamBoard1PrimaryPartDisplayLists,
    gJamBoard2PrimaryPartDisplayLists,
    gJamBoard3PrimaryPartDisplayLists,
};

DisplayLists *gJamBoardSecondaryPartDisplayListTables[] = {
    gJamBoard0SecondaryPartDisplayLists,
    gJamBoard1SecondaryPartDisplayLists,
    gJamBoard2SecondaryPartDisplayLists,
    gJamBoard3SecondaryPartDisplayLists,
};

DisplayLists *gLindaBoardPrimaryPartDisplayListTables[] = {
    gLindaBoard0PrimaryPartDisplayLists,
    gLindaBoard1PrimaryPartDisplayLists,
    gLindaBoard2PrimaryPartDisplayLists,
    gLindaBoard3PrimaryPartDisplayLists,
};

DisplayLists *gLindaBoardSecondaryPartDisplayListTables[] = {
    gLindaBoard0SecondaryPartDisplayLists,
    gLindaBoard1SecondaryPartDisplayLists,
    gLindaBoard2SecondaryPartDisplayLists,
    gLindaBoard3SecondaryPartDisplayLists,
};

DisplayLists *gTommyBoardPrimaryPartDisplayListTables[] = {
    gTommyBoard0PrimaryPartDisplayLists,
    gTommyBoard1PrimaryPartDisplayLists,
    gTommyBoard2PrimaryPartDisplayLists,
    gTommyBoard3PrimaryPartDisplayLists,
};

DisplayLists *gTommyBoardSecondaryPartDisplayListTables[] = {
    gTommyBoard0SecondaryPartDisplayLists,
    gTommyBoard1SecondaryPartDisplayLists,
    gTommyBoard2SecondaryPartDisplayLists,
    gTommyBoard3SecondaryPartDisplayLists,
};

DisplayLists *gWendyBoardPrimaryPartDisplayListTables[] = {
    gWendyBoard0PrimaryPartDisplayLists,
    gWendyBoard1PrimaryPartDisplayLists,
    gWendyBoard2PrimaryPartDisplayLists,
    gWendyBoard3PrimaryPartDisplayLists,
};

DisplayLists *gWendyBoardSecondaryPartDisplayListTables[] = {
    gWendyBoard0SecondaryPartDisplayLists,
    gWendyBoard1SecondaryPartDisplayLists,
    gWendyBoard2SecondaryPartDisplayLists,
    gWendyBoard3SecondaryPartDisplayLists,
};

DisplayLists *gDamienBoardPrimaryPartDisplayListTables[] = {
    gDamienBoard0PrimaryPartDisplayLists,
    gDamienBoard1PrimaryPartDisplayLists,
    gDamienBoard2PrimaryPartDisplayLists,
    gDamienBoard3PrimaryPartDisplayLists,
};

DisplayLists *gDamienBoardSecondaryPartDisplayListTables[] = {
    gDamienBoard0SecondaryPartDisplayLists,
    gDamienBoard1SecondaryPartDisplayLists,
    gDamienBoard2SecondaryPartDisplayLists,
    gDamienBoard3SecondaryPartDisplayLists,
};

DisplayLists *gPenBoardPrimaryPartDisplayListTables[] = {
    gPenBoard0PrimaryPartDisplayLists,
    gPenBoard1PrimaryPartDisplayLists,
    gPenBoard2PrimaryPartDisplayLists,
    gPenBoard3PrimaryPartDisplayLists,
};

DisplayLists *gPenBoardSecondaryPartDisplayListTables[] = {
    gPenBoard0SecondaryPartDisplayLists,
    gPenBoard1SecondaryPartDisplayLists,
    gPenBoard2SecondaryPartDisplayLists,
    gPenBoard3SecondaryPartDisplayLists,
};

DisplayLists *gDogBoardPrimaryPartDisplayListTables[] = {
    gDogBoard0PrimaryPartDisplayLists,
    gDogBoard1PrimaryPartDisplayLists,
    gDogBoard2PrimaryPartDisplayLists,
    gDogBoard3PrimaryPartDisplayLists,
};

DisplayLists *gDogBoardSecondaryPartDisplayListTables[] = {
    gDogBoard0SecondaryPartDisplayLists,
    gDogBoard1SecondaryPartDisplayLists,
    gDogBoard2SecondaryPartDisplayLists,
    gDogBoard3SecondaryPartDisplayLists,
};

DisplayLists *gDinosBoardPartDisplayListTables[] = {
    gDinosBoardPrimaryPartDisplayLists,
    gDinosBoardPrimaryPartDisplayLists,
    gDinosBoardPrimaryPartDisplayLists,
    gDinosBoardPrimaryPartDisplayLists,
};

DisplayLists *gSnowBoardPartDisplayListTables[] = {
    gSnowBoardPrimaryPartDisplayLists,
    gSnowBoardPrimaryPartDisplayLists,
    gSnowBoardPrimaryPartDisplayLists,
    gSnowBoardPrimaryPartDisplayLists,
};

DisplayLists *gRoboBoardPartDisplayListTables[] = {
    gRoboBoardPrimaryPartDisplayLists,
    gRoboBoardPrimaryPartDisplayLists,
    gRoboBoardPrimaryPartDisplayLists,
    gRoboBoardPrimaryPartDisplayLists,
};

DisplayLists **gCharacterBoardPartDisplayListTables[] = {
    gSlashBoardPrimaryPartDisplayListTables,  gNancyBoardPrimaryPartDisplayListTables,
    gJamBoardPrimaryPartDisplayListTables,    gLindaBoardPrimaryPartDisplayListTables,
    gTommyBoardPrimaryPartDisplayListTables,  gWendyBoardPrimaryPartDisplayListTables,
    gDamienBoardPrimaryPartDisplayListTables, gPenBoardPrimaryPartDisplayListTables,
    gDogBoardPrimaryPartDisplayListTables,    gDinosBoardPartDisplayListTables,
    gSnowBoardPartDisplayListTables,          gRoboBoardPartDisplayListTables,
};

DisplayLists **gCharacterBoardSecondaryPartDisplayListTables[] = {
    gSlashBoardSecondaryPartDisplayListTables,
    gNancyBoardSecondaryPartDisplayListTables,
    gJamBoardSecondaryPartDisplayListTables,
    gLindaBoardSecondaryPartDisplayListTables,
    gTommyBoardSecondaryPartDisplayListTables,
    gWendyBoardSecondaryPartDisplayListTables,
    gDamienBoardSecondaryPartDisplayListTables,
    gPenBoardSecondaryPartDisplayListTables,
    gDogBoardSecondaryPartDisplayListTables,
    gDinosBoardPartDisplayListTables,
    gSnowBoardPartDisplayListTables,
    gRoboBoardPartDisplayListTables,
};

CompressedAssetMeta gCharacterRaceAnimationAssets[] = {
    { &SLASH_RACE_ANIMATION_DATA_ROM_START,  &SLASH_RACE_ANIMATION_DATA_ROM_END,  0xAAFA },
    { &NANCY_RACE_ANIMATION_DATA_ROM_START,  &NANCY_RACE_ANIMATION_DATA_ROM_END,  0xB03C },
    { &JAM_RACE_ANIMATION_DATA_ROM_START,    &JAM_RACE_ANIMATION_DATA_ROM_END,    0xB078 },
    { &LINDA_RACE_ANIMATION_DATA_ROM_START,  &LINDA_RACE_ANIMATION_DATA_ROM_END,  0xB4B6 },
    { &TOMMY_RACE_ANIMATION_DATA_ROM_START,  &TOMMY_RACE_ANIMATION_DATA_ROM_END,  0xC2D4 },
    { &WENDY_RACE_ANIMATION_DATA_ROM_START,  &WENDY_RACE_ANIMATION_DATA_ROM_END,  0xB79E },
    { &DAMIEN_RACE_ANIMATION_DATA_ROM_START, &DAMIEN_RACE_ANIMATION_DATA_ROM_END, 0xCA78 },
    { &PEN_RACE_ANIMATION_DATA_ROM_START,    &PEN_RACE_ANIMATION_DATA_ROM_END,    0xB678 },
    { &DOG_RACE_ANIMATION_DATA_ROM_START,    &DOG_RACE_ANIMATION_DATA_ROM_END,    0xBE9A },
    { &DINOS_RACE_ANIMATION_DATA_ROM_START,  &DINOS_RACE_ANIMATION_DATA_ROM_END,  0x1DD0 },
    { &SLASH_RACE_ANIMATION_DATA_ROM_START,  &SLASH_RACE_ANIMATION_DATA_ROM_END,  0xAAFA },
    { &ROBO_RACE_ANIMATION_DATA_ROM_START,   &ROBO_RACE_ANIMATION_DATA_ROM_END,   0x3094 },
};

CompressedAssetMeta gCharacterFaceIconAssets[] = {
    { &SLASH_FACE_ICON_ROM_START,  &SLASH_FACE_ICON_ROM_END,  0x1F8 },
    { &NANCY_FACE_ICON_ROM_START,  &NANCY_FACE_ICON_ROM_END,  0x1F8 },
    { &JAM_FACE_ICON_ROM_START,    &JAM_FACE_ICON_ROM_END,    0x1F8 },
    { &LINDA_FACE_ICON_ROM_START,  &LINDA_FACE_ICON_ROM_END,  0x1F8 },
    { &TOMMY_FACE_ICON_ROM_START,  &TOMMY_FACE_ICON_ROM_END,  0x1F8 },
    { &WENDY_FACE_ICON_ROM_START,  &WENDY_FACE_ICON_ROM_END,  0x1F8 },
    { &DAMIEN_FACE_ICON_ROM_START, &DAMIEN_FACE_ICON_ROM_END, 0x1F8 },
    { &PEN_FACE_ICON_ROM_START,    &PEN_FACE_ICON_ROM_END,    0x1F8 },
    { &DOG_FACE_ICON_ROM_START,    &DOG_FACE_ICON_ROM_END,    0x1F8 },
    { &DINOS_FACE_ICON_ROM_START,  &DINOS_FACE_ICON_ROM_END,  0x2A8 },
    { &SNOW_FACE_ICON_ROM_START,   &SNOW_FACE_ICON_ROM_END,   0x210 },
    { &DAMIEN_FACE_ICON_ROM_START, &DAMIEN_FACE_ICON_ROM_END, 0x1F8 },
};

AssetMeta gCostumeSlotDisplayListAssets[] = {
    { &COSTUME_SLOT_00_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_00_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_01_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_01_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_02_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_02_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_03_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_03_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_04_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_04_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_05_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_05_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_06_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_06_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_07_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_07_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_08_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_08_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_09_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_09_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_10_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_10_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_11_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_11_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_12_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_12_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_13_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_13_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_14_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_14_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_15_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_15_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_16_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_16_DISPLAY_LIST_ROM_END },
    { &COSTUME_SLOT_17_DISPLAY_LIST_ROM_START, &COSTUME_SLOT_17_DISPLAY_LIST_ROM_END },
};

CompressedAssetMeta gCostumeSlotCompressedDataAssets[] = {
    { &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_END, 0x320 },
    { &COSTUME_SLOT_01_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_01_COMPRESSED_DATA_ROM_END, 0x300 },
    { &COSTUME_SLOT_02_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_02_COMPRESSED_DATA_ROM_END, 0x300 },
    { &COSTUME_SLOT_03_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_03_COMPRESSED_DATA_ROM_END, 0x360 },
    { &COSTUME_SLOT_04_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_04_COMPRESSED_DATA_ROM_END, 0x360 },
    { &COSTUME_SLOT_05_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_05_COMPRESSED_DATA_ROM_END, 0x360 },
    { &COSTUME_SLOT_06_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_06_COMPRESSED_DATA_ROM_END, 0x2E0 },
    { &COSTUME_SLOT_07_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_07_COMPRESSED_DATA_ROM_END, 0x2E0 },
    { &COSTUME_SLOT_08_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_08_COMPRESSED_DATA_ROM_END, 0x2E0 },
    { &COSTUME_SLOT_09_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_09_COMPRESSED_DATA_ROM_END, 0xC00 },
    { &COSTUME_SLOT_10_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_10_COMPRESSED_DATA_ROM_END, 0xA00 },
    { &COSTUME_SLOT_11_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_11_COMPRESSED_DATA_ROM_END, 0xA90 },
    { &COSTUME_SLOT_12_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_12_COMPRESSED_DATA_ROM_END, 0x9C0 },
    { &COSTUME_SLOT_13_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_13_COMPRESSED_DATA_ROM_END, 0xA20 },
    { &COSTUME_SLOT_14_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_14_COMPRESSED_DATA_ROM_END, 0xA20 },
    { &COSTUME_SLOT_15_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_15_COMPRESSED_DATA_ROM_END, 0xD60 },
    { &COSTUME_SLOT_16_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_16_COMPRESSED_DATA_ROM_END, 0xB00 },
    { &COSTUME_SLOT_17_COMPRESSED_DATA_ROM_START, &COSTUME_SLOT_17_COMPRESSED_DATA_ROM_END, 0xBA0 }
};

CompressedAssetMeta gCostumePaletteAssets[] = {
    { &PALETTE_00_ROM_START, &PALETTE_00_ROM_END, 0x820 },
    { &PALETTE_01_ROM_START, &PALETTE_01_ROM_END, 0x820 },
    { &PALETTE_02_ROM_START, &PALETTE_02_ROM_END, 0x820 },
    { &PALETTE_03_ROM_START, &PALETTE_03_ROM_END, 0x820 },
    { &PALETTE_04_ROM_START, &PALETTE_04_ROM_END, 0x820 },
    { &PALETTE_05_ROM_START, &PALETTE_05_ROM_END, 0x820 },
    { &PALETTE_06_ROM_START, &PALETTE_06_ROM_END, 0x820 },
    { &PALETTE_07_ROM_START, &PALETTE_07_ROM_END, 0x820 },
    { &PALETTE_08_ROM_START, &PALETTE_08_ROM_END, 0x820 },
    { &PALETTE_09_ROM_START, &PALETTE_09_ROM_END, 0x820 },
    { &PALETTE_10_ROM_START, &PALETTE_10_ROM_END, 0x820 },
    { &PALETTE_11_ROM_START, &PALETTE_11_ROM_END, 0x820 },
    { &PALETTE_12_ROM_START, &PALETTE_12_ROM_END, 0x820 },
    { &PALETTE_13_ROM_START, &PALETTE_13_ROM_END, 0x820 },
    { &PALETTE_14_ROM_START, &PALETTE_14_ROM_END, 0x820 },
    { &PALETTE_15_ROM_START, &PALETTE_15_ROM_END, 0x820 },
};

AssetMeta *gCostumeSlotDisplayListTable[] = {
    &D_8009A550_9B150[0],  &D_8009A550_9B150[2],  &D_8009A550_9B150[4],  &D_8009A550_9B150[6],  &D_8009A550_9B150[8],
    &D_8009A550_9B150[10], &D_8009A550_9B150[12], &D_8009A550_9B150[14], &D_8009A550_9B150[16], &D_8009A550_9B150[18],
    &D_8009A550_9B150[20], &D_8009A550_9B150[22], &D_8009A550_9B150[24], &D_8009A550_9B150[26], &D_8009A550_9B150[28],
    &D_8009A550_9B150[30], &D_8009A550_9B150[32], &D_8009A550_9B150[34],
};

CompressedAssetMeta gBossHudAssetTable[] = {
    { &BOSS_HUD_ASSET_00_ROM_START, &BOSS_HUD_ASSET_00_ROM_END, 0x3D0 },
    { &BOSS_HUD_ASSET_01_ROM_START, &BOSS_HUD_ASSET_01_ROM_END, 0x5D0 },
    { &BOSS_HUD_ASSET_02_ROM_START, &BOSS_HUD_ASSET_02_ROM_END, 0x6E0 },
    { &BOSS_HUD_ASSET_03_ROM_START, &BOSS_HUD_ASSET_03_ROM_END, 0x6E0 },
    { &BOSS_HUD_ASSET_04_ROM_START, &BOSS_HUD_ASSET_04_ROM_END, 0x790 },
    { &BOSS_HUD_ASSET_05_ROM_START, &BOSS_HUD_ASSET_05_ROM_END, 0x8A0 },
    { &BOSS_HUD_ASSET_06_ROM_START, &BOSS_HUD_ASSET_06_ROM_END, 0x6B0 },
    { &BOSS_HUD_ASSET_07_ROM_START, &BOSS_HUD_ASSET_07_ROM_END, 0x6B0 },
    { &BOSS_HUD_ASSET_08_ROM_START, &BOSS_HUD_ASSET_08_ROM_END, 0x7C0 },
    { &BOSS_HUD_ASSET_09_ROM_START, &BOSS_HUD_ASSET_09_ROM_END, 0x700 },
    { &BOSS_HUD_ASSET_10_ROM_START, &BOSS_HUD_ASSET_10_ROM_END, 0x710 },
    { &BOSS_HUD_ASSET_11_ROM_START, &BOSS_HUD_ASSET_11_ROM_END, 0x710 },
    { &BOSS_HUD_ASSET_12_ROM_START, &BOSS_HUD_ASSET_12_ROM_END, 0x5C0 },
    { &BOSS_HUD_ASSET_13_ROM_START, &BOSS_HUD_ASSET_13_ROM_END, 0x5C0 },
    { &BOSS_HUD_ASSET_14_ROM_START, &BOSS_HUD_ASSET_14_ROM_END, 0x2E0 },
    { &BOSS_HUD_ASSET_15_ROM_START, &BOSS_HUD_ASSET_15_ROM_END, 0x100 },
};

CompressedAssetMeta gGhostInputRecordAssets[] = {
    { &GHOST_INPUT_RECORD_00_ROM_START, &GHOST_INPUT_RECORD_00_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_01_ROM_START, &GHOST_INPUT_RECORD_01_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_02_ROM_START, &GHOST_INPUT_RECORD_02_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_03_ROM_START, &GHOST_INPUT_RECORD_03_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_04_ROM_START, &GHOST_INPUT_RECORD_04_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_05_ROM_START, &GHOST_INPUT_RECORD_05_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_06_ROM_START, &GHOST_INPUT_RECORD_06_ROM_END, 0xE10  },
    { &GHOST_INPUT_RECORD_07_ROM_START, &GHOST_INPUT_RECORD_07_ROM_END, 0x3390 },
    { &GHOST_INPUT_RECORD_08_ROM_START, &GHOST_INPUT_RECORD_08_ROM_END, 0x3390 },
    { &GHOST_INPUT_RECORD_09_ROM_START, &GHOST_INPUT_RECORD_09_ROM_END, 0x3390 },
    { &GHOST_INPUT_RECORD_10_ROM_START, &GHOST_INPUT_RECORD_10_ROM_END, 0x3390 },
    { (void *)0x0000,                   (void *)0x0000,                 0x0000 },
};

DisplayLists gSunnyMountainSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x010070D8, (Gfx *)0x010071B0, (Gfx *)0x00000000 },
};

DisplayLists gSunnyMountainFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01007550, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSunnyMountainDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x01007560, (Gfx *)0x010078E0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007A08, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007C08, (Gfx *)0x01007F48, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008028, (Gfx *)0x010081A8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008328, (Gfx *)0x010084A8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008628, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008760, (Gfx *)0x010087E8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x010088C8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008968, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008A08, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x01008DA8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008E88, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSunnyMountainScrollingSceneryDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01009200, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01009950, (Gfx *)0x01009AD8, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01009DF0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandDefaultFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A020, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists1[] = {
    { 0x00000000, (Gfx *)0x0100A110, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists2[] = {
    { 0x00000000, (Gfx *)0x0100A1B8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists3[] = {
    { 0x00000000, (Gfx *)0x0100A240, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists4[] = {
    { 0x00000000, (Gfx *)0x0100A330, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gTurtleIslandDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100A340, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A600, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A8E8, (Gfx *)0x0100AD48, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AE48, (Gfx *)0x0100AFE8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B0E0, (Gfx *)0x0100B280, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B378, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B4D0, (Gfx *)0x0100B558, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B638, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B6D8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B778, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B860, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A070, (Gfx *)0x0100A1F8, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownScrollingSceneryDisplayLists1[] = {
    { 0x00000000, (Gfx *)0x0100A378, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownScrollingSceneryDisplayLists2[] = {
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100A448 },
};

DisplayLists gJingleTownFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A718, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100A728, (Gfx *)0x0100AC08, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AD68, (Gfx *)0x0100AF68, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B0C8, (Gfx *)0x0100B410, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B4F0, (Gfx *)0x0100B6B0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B840, (Gfx *)0x0100BA00, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BB90, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BCD8, (Gfx *)0x0100BD60, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BE48, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BF00, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BFB8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownBossSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01009CC8, (Gfx *)0x01009E50, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownBossScrollingSceneryDisplayLists1[] = {
    { 0x00000000, (Gfx *)0x01009FC0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownBossScrollingSceneryDisplayLists2[] = {
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100A090 },
};

DisplayLists gJingleTownBossFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A350, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gJingleTownBossDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100A360, (Gfx *)0x0100A858, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A9C8, (Gfx *)0x0100ABD0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AD60, (Gfx *)0x0100B0C0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B1A0, (Gfx *)0x0100B338, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B4E0, (Gfx *)0x0100B678, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B820, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B958, (Gfx *)0x0100B9E0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BAC8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BB78, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BC28, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gWendysHouseSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A3E0, (Gfx *)0x0100A600, (Gfx *)0x0100A6E0 },
};

DisplayLists gWendysHouseFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A870, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gWendysHouseScrollingSceneryDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A918, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gWendysHouseDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100A928, (Gfx *)0x0100AD28, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AE50, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B050, (Gfx *)0x0100B390, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100B470, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100B688, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B8A0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100B9E8, (Gfx *)0x0100BA70, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BB58, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BC08, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BCB8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BE00, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100BF00, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C118, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gLindasCastleSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100C0B0, (Gfx *)0x0100C230, (Gfx *)0x00000000 },
};

DisplayLists gLindasCastleFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100C728, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gLindasCastleDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100C738, (Gfx *)0x0100C8D8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C9C0, (Gfx *)0x0100CBD8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CCF0, (Gfx *)0x0100CDC0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CEA8, (Gfx *)0x0100CFC0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D0A8, (Gfx *)0x0100D1C0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D2B8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D3E8, (Gfx *)0x0100D478, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D558, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D5F8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D698, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gCrazyJungleSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01007F88, (Gfx *)0x010080B0, (Gfx *)0x01008198 },
};

DisplayLists gCrazyJungleFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x010083F0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gCrazyJungleDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x01008400, (Gfx *)0x010086E0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008950, (Gfx *)0x01008D08, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008DF0, (Gfx *)0x010090A0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009308, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009498, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009628, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x010097C0, (Gfx *)0x01009848, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009928, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x010099C8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009A68, (Gfx *)0x01009BD8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009D30, (Gfx *)0x01009F10, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A068, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A148, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

s32 D_80095340_95F40[] = {
    0x00000000, 0x0100A228, 0x00000000, 0x00000000, 0x00000000, 0x0100A2C8, 0x00000000, 0x00000000,
};

DisplayLists gCrazyJungleScrollingSceneryDisplayLists1[] = {
    { 0x00000000, (Gfx *)0x0100A7D0, (Gfx *)0x00000000, (Gfx *)0x0100A7E8 },
};

DisplayLists gCrazyJungleScrollingSceneryDisplayLists2[] = {
    { 0x00000000, (Gfx *)0x0100AA70, (Gfx *)0x00000000, (Gfx *)0x0100AA80 },
};

DisplayLists gCrazyJungleScrollingSceneryDisplayLists3[] = {
    { 0x00000000, (Gfx *)0x0100AB18, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gCrazyJungleBossSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01009300, (Gfx *)0x01009430, (Gfx *)0x00000000 },
};

DisplayLists gCrazyJungleBossFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01009888, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gCrazyJungleBossDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x01009898, (Gfx *)0x01009B70, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009DE0, (Gfx *)0x0100A190, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A278, (Gfx *)0x0100A520, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A788, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A918, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AAA8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AC40, (Gfx *)0x0100ACC8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100ADA8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100AE48, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

s32 D_80095440_96040[] = {
    0x00000000, 0x0100AEE8, 0x00000000, 0x00000000, 0x00000000, 0x0100AF88, 0x00000000, 0x00000000,
};

DisplayLists gCrazyJungleBossScrollingSceneryDisplayLists1[] = {
    { 0x00000000, (Gfx *)0x0100B4B0, (Gfx *)0x00000000, (Gfx *)0x0100B4C8 },
};

DisplayLists gCrazyJungleBossScrollingSceneryDisplayLists2[] = {
    { 0x00000000, (Gfx *)0x0100B790, (Gfx *)0x00000000, (Gfx *)0x0100B7A0 },
};

DisplayLists gCrazyJungleBossScrollingSceneryDisplayLists3[] = {
    { 0x00000000, (Gfx *)0x0100B838, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gStarlightHighwaySkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100B688, (Gfx *)0x0100B8A0, (Gfx *)0x0100B9A0 },
};

DisplayLists gStarlightHighwayFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100BDC0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gStarlightHighwayDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100BDD0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C028, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C280, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C4D8, (Gfx *)0x0100C678, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C770, (Gfx *)0x0100C910, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CA08, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CBB0, (Gfx *)0x0100CC38, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CD18, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CDC8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100CE78, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100CFE0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D128, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D270, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D3E8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D560, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D630, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gDebugDisplayConfig = { 0x00000000, NULL, NULL, (Gfx *)0x0100D798 };

DisplayLists gStarlightHighwayScrollingSceneryDisplayLists1[] = {
    { 0x00000000, (Gfx *)0x0100DDF0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gStarlightHighwayScrollingSceneryDisplayLists2[] = {
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100E200 },
};

DisplayLists gStarlightHighwayScrollingSceneryDisplayLists3[] = {
    { 0x00000000, (Gfx *)0x0100E558, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gHauntedHouseSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100B878, (Gfx *)0x0100BB48, (Gfx *)0x00000000 },
};

DisplayLists gHauntedHouseFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100BE90, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gHauntedHouseDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100BEA0, (Gfx *)0x0100C530, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C648, (Gfx *)0x0100C7D8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100C8C0, (Gfx *)0x0100CF78, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D058, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D1F0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D388, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D4B8, (Gfx *)0x0100D5B8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D698, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D748, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100D7F8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

s32 D_800956B0_962B0[] = {
    0x00000000,
    0x0100DD30,
    0x00000000,
    0x00000000,
};

DisplayLists gIceLandSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01008CD8, (Gfx *)0x01008E18, (Gfx *)0x01008EB8 },
};

DisplayLists gIceLandDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x01008EE0, (Gfx *)0x010091A8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009328, (Gfx *)0x01009770, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009968, (Gfx *)0x01009B60, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009C58, (Gfx *)0x01009D50, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009E90, (Gfx *)0x01009F88, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A0C8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A228, (Gfx *)0x0100A2B0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A390, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A430, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A4D0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

s32 D_80095770_96370[] = {
    0x00000000,
    0x0100A678,
    0x0100A718,
    0x00000000,
};

DisplayLists gIceLandFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100AAA0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gIceLandBossSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01008920, (Gfx *)0x01008A60, (Gfx *)0x01008B00 },
};

DisplayLists gIceLandBossDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x01008B28, (Gfx *)0x01008DF0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008F78, (Gfx *)0x010093C0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x010095B8, (Gfx *)0x010097C0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x010098A8, (Gfx *)0x01009990, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009AC0, (Gfx *)0x01009BA8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009CD8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009E30, (Gfx *)0x01009EB8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01009F98, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100A038, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

s32 D_80095830_96430[] = {
    0x00000000,
    0x0100A0D8,
    0x0100A178,
    0x00000000,
};

DisplayLists gIceLandBossFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100A500, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetSpeedCrossSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100D318, (Gfx *)0x0100D4E0, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetSpeedCrossScrollingSceneryDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100D7A8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetSpeedCrossFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100DA80, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetSpeedCrossDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100DA90, (Gfx *)0x0100DEA0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100DFA0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E180, (Gfx *)0x0100E590, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E670, (Gfx *)0x0100E7D0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E8C8, (Gfx *)0x0100EA28, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100EB20, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100EC80, (Gfx *)0x0100ED18, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100EE00, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100EEB0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100EF60, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetShotCrossSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100CDF0, (Gfx *)0x0100CFB0, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetShotCrossScrollingSceneryDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100D270, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetShotCrossFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x0100D550, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSnowboardStreetShotCrossDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x0100D560, (Gfx *)0x0100D960, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100DA60, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100DC40, (Gfx *)0x0100E038, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E118, (Gfx *)0x0100E278, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E370, (Gfx *)0x0100E4D0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E5C8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E710, (Gfx *)0x0100E7A8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E890, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x0100E950, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x00000000, (Gfx *)0x0100EA00, (Gfx *)0x00000000 },
};

DisplayLists gXCrossSkyDisplayLists[] = {
    { 0x00000000, (Gfx *)0x01006DC0, (Gfx *)0x01006F20, (Gfx *)0x00000000 },
};

DisplayLists gXCrossFogDisplayLists[] = {
    { 0x00000000, (Gfx *)0x010071E8, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gXCrossDisplayListTable[] = {
    { 0x00000000, (Gfx *)0x010071F8, (Gfx *)0x01007488, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007588, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x010077B0, (Gfx *)0x010079F0, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007AD0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007C58, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007DE0, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01007F28, (Gfx *)0x01007FB8, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008098, (Gfx *)0x00000000, (Gfx *)0x00000000 },
    { 0x00000000, (Gfx *)0x01008148, (Gfx *)0x00000000, (Gfx *)0x00000000 },
};

DisplayLists gSlashBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_18A0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_1928, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_19E0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_1B58, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_1C88, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_1D40, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_1EB8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_1FE8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2180, SLASH_BOARD0_DISPLAY_LIST_part_2320, NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2460, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2540, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_25E8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2690, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_27A0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2848, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_28F0, NULL,                                NULL }
};

DisplayLists gSlashBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2A18, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2A58, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2A98, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2AD8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2B18, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2B58, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2B98, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2BD8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2C18, SLASH_BOARD0_DISPLAY_LIST_part_2C58, NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2C98, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2CD8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2D18, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2D58, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2D98, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2DD8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD0_DISPLAY_LIST_part_2E18, NULL,                                NULL }
};

DisplayLists gSlashBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1720, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_17A8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_18A0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1998, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1AC8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1BC0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1CB8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1DE8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_1F70, SLASH_BOARD1_DISPLAY_LIST_part_2168, NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_22A8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2388, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2470, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2518, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2630, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2718, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_27C0, NULL,                                NULL }
};

DisplayLists gSlashBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_28F0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2930, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2970, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_29B0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_29F0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2A30, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2A70, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2AB0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2AF0, SLASH_BOARD1_DISPLAY_LIST_part_2B30, NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2B70, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2BB0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2BF0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2C30, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2C70, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2CB0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD1_DISPLAY_LIST_part_2CF0, NULL,                                NULL }
};

DisplayLists gSlashBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1AC0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1B48, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1C00, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1CE8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1E18, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1ED0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_1FB8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_20E8, SLASH_BOARD2_DISPLAY_LIST_part_2178, NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_22D0, SLASH_BOARD2_DISPLAY_LIST_part_2640, NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2800, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_28E0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2988, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2B00, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2BD8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2C80, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2DF8, NULL,                                NULL }
};

DisplayLists gSlashBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2FB0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_2FF0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3030, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3070, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_30B0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_30F0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3130, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3170, SLASH_BOARD2_DISPLAY_LIST_part_31B0, NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_31F0, SLASH_BOARD2_DISPLAY_LIST_part_3230, NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3270, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_32B0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_32F0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3330, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_3370, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_33B0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD2_DISPLAY_LIST_part_33F0, NULL,                                NULL }
};

DisplayLists gSlashBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1688, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1710, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_17C8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_18B8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1968, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1A20, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1B10, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1BC0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1C90, SLASH_BOARD3_DISPLAY_LIST_part_1E30, NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_1FF8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_20D8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_21C8, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2280, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_23A0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2490, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2548, NULL,                                NULL }
};

DisplayLists gSlashBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2680, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_26C0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2700, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2740, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2780, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_27C0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2800, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2840, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2880, SLASH_BOARD3_DISPLAY_LIST_part_28C0, NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2900, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2940, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2980, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_29C0, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2A00, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2A40, NULL,                                NULL },
    { 0x00000000, SLASH_BOARD3_DISPLAY_LIST_part_2A80, NULL,                                NULL }
};

DisplayLists gNancyBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0070, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0128, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0238, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_03A0, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0458, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0568, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_06D0, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0840, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0AE8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0B90, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0C28, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0CF0, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0DD8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0E70, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0F30, NULL, NULL }
};

DisplayLists gNancyBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_26B8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_26F8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2738, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2778, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_27B8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_27F8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2838, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2878, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_28B8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_28F8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2938, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2978, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_29B8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_29F8, NULL, NULL },
    { 0x00000000, NANCY_BOARD0_DISPLAY_LIST_part_2A38, NULL, NULL }
};

DisplayLists gNancyBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_00C8, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0178, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0258, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_03F0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_04A0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0580, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0738, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0838, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0A88, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0B30, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0BD0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0C78, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0DE8, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0E88, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_0F30, NULL, NULL }
};

DisplayLists gNancyBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2840, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2880, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_28C0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2900, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2940, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2980, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_29C0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2A00, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2A40, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2A80, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2AC0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2B00, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2B40, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2B80, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2BC0, NULL, NULL },
    { 0x00000000, NANCY_BOARD1_DISPLAY_LIST_part_2C00, NULL, NULL }
};

DisplayLists gNancyBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0148, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0200, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_02E0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0468, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0520, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0600, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0798, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0898, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0AE0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0B88, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0C28, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0D00, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0E50, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0EF0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_0FC8, NULL, NULL }
};

DisplayLists gNancyBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2990, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_29D0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2A10, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2A50, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2A90, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2AD0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2B10, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2B50, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2B90, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2BD0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2C10, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2C50, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2C90, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2CD0, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2D10, NULL, NULL },
    { 0x00000000, NANCY_BOARD2_DISPLAY_LIST_part_2D50, NULL, NULL }
};

DisplayLists gNancyBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_00E8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_01A0, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0290, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0368, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0420, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0520, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_05F8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0740, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0990, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0A38, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0AD8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0B88, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0C60, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0D00, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_0DB0, NULL, NULL }
};

DisplayLists gNancyBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2458, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2498, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_24D8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2518, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2558, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2598, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_25D8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2618, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2658, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2698, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_26D8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2718, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2758, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2798, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_27D8, NULL, NULL },
    { 0x00000000, NANCY_BOARD3_DISPLAY_LIST_part_2818, NULL, NULL }
};

DisplayLists gJamBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0088, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0140, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0238, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0398, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0450, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0548, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_06A8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0830, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0A58, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0BA0, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0C70, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0D18, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0E20, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0EF0, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_0F98, NULL, NULL }
};

DisplayLists gJamBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_26B8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_26F8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2738, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2778, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_27B8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_27F8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2838, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2878, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_28B8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_28F8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2938, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2978, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_29B8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_29F8, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2A38, NULL, NULL },
    { 0x00000000, JAM_BOARD0_DISPLAY_LIST_part_2A78, NULL, NULL }
};

DisplayLists gJamBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_1838, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_18C0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_19B8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_1B28, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_1C58, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_1D50, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_1EC0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_1FD0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2160, JAM_BOARD1_DISPLAY_LIST_part_2300, NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2448, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2590, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_26F8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_27A0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2920, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2A88, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2B30, NULL,                              NULL }
};

DisplayLists gJamBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2CC8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2D08, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2D48, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2D88, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2DC8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2E08, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2E48, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2E88, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2EC8, JAM_BOARD1_DISPLAY_LIST_part_2F08, NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2F48, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2F88, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_2FC8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_3008, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_3048, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_3088, NULL,                              NULL },
    { 0x00000000, JAM_BOARD1_DISPLAY_LIST_part_30C8, NULL,                              NULL }
};

DisplayLists gJamBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0088, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0140, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_02E8, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_03A0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0498, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0558, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0718, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_09C8, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0B10, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0BB8, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0C60, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0D38, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0DE0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_0E88, NULL, NULL }
};

DisplayLists gJamBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2470, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_24B0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_24F0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2530, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2570, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_25B0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_25F0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2630, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2670, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_26B0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_26F0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2730, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2770, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_27B0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_27F0, NULL, NULL },
    { 0x00000000, JAM_BOARD2_DISPLAY_LIST_part_2830, NULL, NULL }
};

DisplayLists gJamBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0000, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0088, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0140, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0230, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_02F0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_03A8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0498, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0558, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0668, JAM_BOARD3_DISPLAY_LIST_part_0948, NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0AA8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0BF0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0CA8, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0D70, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0E90, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_0F48, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_1010, NULL,                              NULL }
};

DisplayLists gJamBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_29E0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2A20, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2A60, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2AA0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2AE0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2B20, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2B60, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2BA0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2BE0, JAM_BOARD3_DISPLAY_LIST_part_2C20, NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2C60, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2CA0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2CE0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2D20, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2D60, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2DA0, NULL,                              NULL },
    { 0x00000000, JAM_BOARD3_DISPLAY_LIST_part_2DE0, NULL,                              NULL }
};

DisplayLists gLindaBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_00E8, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_01B8, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_02B0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0468, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0538, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0630, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_07E8, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0920, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0B40, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0BC0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0C68, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0D28, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0E10, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0EB8, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_0F78, NULL, NULL }
};

DisplayLists gLindaBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_27F0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2830, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2870, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_28B0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_28F0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2930, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2970, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_29B0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_29F0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2A30, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2A70, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2AB0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2AF0, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2B30, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2B70, NULL, NULL },
    { 0x00000000, LINDA_BOARD0_DISPLAY_LIST_part_2BB0, NULL, NULL }
};

DisplayLists gLindaBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_00C8, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0178, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0248, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0350, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0400, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_04D0, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_05D8, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0700, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0920, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_09A0, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0A58, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0B00, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0C70, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0D28, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_0DE8, NULL, NULL }
};

DisplayLists gLindaBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2598, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_25D8, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2618, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2658, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2698, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_26D8, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2718, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2758, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2798, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_27D8, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2818, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2858, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2898, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_28D8, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2918, NULL, NULL },
    { 0x00000000, LINDA_BOARD1_DISPLAY_LIST_part_2958, NULL, NULL }
};

DisplayLists gLindaBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_00C8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0178, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0228, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0308, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_03B8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0468, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0560, LINDA_BOARD2_DISPLAY_LIST_part_0658, NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0750, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0960, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_09E0, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0A88, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0B30, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0CA0, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0D60, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_0E08, NULL,                                NULL }
};

DisplayLists gLindaBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2678, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_26B8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_26F8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2738, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2778, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_27B8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_27F8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2838, LINDA_BOARD2_DISPLAY_LIST_part_2878, NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_28B8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_28F8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2938, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2978, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_29B8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_29F8, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2A38, NULL,                                NULL },
    { 0x00000000, LINDA_BOARD2_DISPLAY_LIST_part_2A78, NULL,                                NULL }
};

DisplayLists gLindaBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_00F8, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_01A8, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0260, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0338, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_03E8, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_04A0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0578, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_06D0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_08F0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0970, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0A18, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0AB8, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0BB0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0C58, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_0CF8, NULL, NULL }
};

DisplayLists gLindaBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2390, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_23D0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2410, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2450, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2490, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_24D0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2510, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2550, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2590, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_25D0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2610, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2650, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2690, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_26D0, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2710, NULL, NULL },
    { 0x00000000, LINDA_BOARD3_DISPLAY_LIST_part_2750, NULL, NULL }
};

DisplayLists gTommyBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1A98, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1B08, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1BE8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1CB0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1E28, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1F08, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_1FD0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2148, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2308, TOMMY_BOARD0_DISPLAY_LIST_part_2438, NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2630, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2790, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_28A8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_29C0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2AC8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2BE0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2CF8, NULL,                                NULL }
};

DisplayLists gTommyBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2E18, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2E58, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2E98, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2ED8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2F18, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2F58, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2F98, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_2FD8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_3018, TOMMY_BOARD0_DISPLAY_LIST_part_3058, NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_3098, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_30D8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_3118, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_3158, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_3198, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_31D8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD0_DISPLAY_LIST_part_3218, NULL,                                NULL }
};

DisplayLists gTommyBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0070, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0130, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0220, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0398, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0458, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0548, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_06C0, TOMMY_BOARD1_DISPLAY_LIST_part_0758, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_08F8, TOMMY_BOARD1_DISPLAY_LIST_part_0A58, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0C68, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0DE8, TOMMY_BOARD1_DISPLAY_LIST_part_0E78, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_0F70, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_1018, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_10F8, TOMMY_BOARD1_DISPLAY_LIST_part_1188, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_1280, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_1328, NULL,                                NULL }
};

DisplayLists gTommyBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_2FA0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_2FE0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3020, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3060, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_30A0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_30E0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3120, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3160, TOMMY_BOARD1_DISPLAY_LIST_part_31A0, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_31E0, TOMMY_BOARD1_DISPLAY_LIST_part_3220, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3260, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_32A0, TOMMY_BOARD1_DISPLAY_LIST_part_32E0, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3320, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3360, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_33A0, TOMMY_BOARD1_DISPLAY_LIST_part_33E0, NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3420, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD1_DISPLAY_LIST_part_3460, NULL,                                NULL }
};

DisplayLists gTommyBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0070, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0150, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0298, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0400, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_04E0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0628, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0790, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0940, TOMMY_BOARD2_DISPLAY_LIST_part_0A18, NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0C50, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0DB0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0E78, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_0FB0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_1138, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_1200, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_12A8, NULL,                                NULL }
};

DisplayLists gTommyBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_2FE8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3028, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3068, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_30A8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_30E8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3128, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3168, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_31A8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_31E8, TOMMY_BOARD2_DISPLAY_LIST_part_3228, NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3268, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_32A8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_32E8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3328, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_3368, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_33A8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD2_DISPLAY_LIST_part_33E8, NULL,                                NULL }
};

DisplayLists gTommyBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0070, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_01E8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0340, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_04A8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0620, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0768, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_08D0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0A98, TOMMY_BOARD3_DISPLAY_LIST_part_0BB0, NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0D50, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0EB0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_0FE8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_1100, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_11E0, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_1318, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_1430, NULL,                                NULL }
};

DisplayLists gTommyBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_30F8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3138, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3178, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_31B8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_31F8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3238, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3278, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_32B8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_32F8, TOMMY_BOARD3_DISPLAY_LIST_part_3338, NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3378, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_33B8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_33F8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3438, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_3478, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_34B8, NULL,                                NULL },
    { 0x00000000, TOMMY_BOARD3_DISPLAY_LIST_part_34F8, NULL,                                NULL }
};

DisplayLists gWendyBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_00B8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0178, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_02E0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0448, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0508, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0670, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_07D8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0958, WENDY_BOARD0_DISPLAY_LIST_part_0A70, NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0C40, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0D08, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0DA8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0ED8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_0FA0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_1040, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_1170, NULL,                                NULL }
};

DisplayLists gWendyBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2C30, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2C70, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2CB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2CF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2D30, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2D70, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2DB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2DF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2E30, WENDY_BOARD0_DISPLAY_LIST_part_2E70, NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2EB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2EF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2F30, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2F70, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2FB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_2FF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD0_DISPLAY_LIST_part_3030, NULL,                                NULL }
};

DisplayLists gWendyBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_00B8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0228, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_02D8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0440, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_05B0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0660, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_07C8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0910, WENDY_BOARD1_DISPLAY_LIST_part_0A20, NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0BF8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0CC0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0D60, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0E78, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_0FC0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_1060, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_1178, NULL,                                NULL }
};

DisplayLists gWendyBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2CF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2D30, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2D70, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2DB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2DF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2E30, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2E70, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2EB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2EF0, WENDY_BOARD1_DISPLAY_LIST_part_2F30, NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2F70, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2FB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_2FF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_3030, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_3070, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_30B0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD1_DISPLAY_LIST_part_30F0, NULL,                                NULL }
};

DisplayLists gWendyBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0070, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0118, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0258, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0340, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_03E8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0530, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0618, WENDY_BOARD2_DISPLAY_LIST_part_0710, NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0818, WENDY_BOARD2_DISPLAY_LIST_part_0940, NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0B18, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0BE0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0C80, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0DB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0E78, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_0F18, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_1048, NULL,                                NULL }
};

DisplayLists gWendyBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2A80, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2AC0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2B00, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2B40, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2B80, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2BC0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2C00, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2C40, WENDY_BOARD2_DISPLAY_LIST_part_2C80, NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2CC0, WENDY_BOARD2_DISPLAY_LIST_part_2D00, NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2D40, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2D80, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2DC0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2E00, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2E40, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2E80, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD2_DISPLAY_LIST_part_2EC0, NULL,                                NULL }
};

DisplayLists gWendyBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0000, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_00B8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0190, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0290, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0390, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0468, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0550, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0650, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0760, WENDY_BOARD3_DISPLAY_LIST_part_08F8, NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0AF0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0BB8, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0C58, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0D00, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0DB0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0E50, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_0EF8, NULL,                                NULL }
};

DisplayLists gWendyBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2700, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2740, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2780, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_27C0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2800, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2840, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2880, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_28C0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2900, WENDY_BOARD3_DISPLAY_LIST_part_2940, NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2980, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_29C0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2A00, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2A40, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2A80, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2AC0, NULL,                                NULL },
    { 0x00000000, WENDY_BOARD3_DISPLAY_LIST_part_2B00, NULL,                                NULL }
};

DisplayLists gDamienBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0000, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_00B8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0170, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0328, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0488, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0540, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_06F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0858, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0AB0, DAMIEN_BOARD0_DISPLAY_LIST_part_0CF8, NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0DF0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0EA0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0F48, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_0FF0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_1188, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_1230, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_12D8, NULL,                                 NULL }
};

DisplayLists gDamienBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_2F20, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_2F60, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_2FA0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_2FE0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_3020, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_3060, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_30A0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_30E0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_3120, DAMIEN_BOARD0_DISPLAY_LIST_part_3160, NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_31A0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_31E0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_3220, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_3260, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_32A0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_32E0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD0_DISPLAY_LIST_part_3320, NULL,                                 NULL }
};

DisplayLists gDamienBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0000, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_00B8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0170, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0328, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0488, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0540, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_06F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0858, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0AB0, DAMIEN_BOARD1_DISPLAY_LIST_part_0CF8, NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0DF0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0EA0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0F48, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_0FF0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_1188, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_1230, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_12D8, NULL,                                 NULL }
};

DisplayLists gDamienBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_2F58, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_2F98, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_2FD8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3018, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3058, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3098, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_30D8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3118, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3158, DAMIEN_BOARD1_DISPLAY_LIST_part_3198, NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_31D8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3218, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3258, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3298, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_32D8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3318, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD1_DISPLAY_LIST_part_3358, NULL,                                 NULL }
};

DisplayLists gDamienBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0000, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0150, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_01F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0350, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0480, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0528, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0680, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_07B0, DAMIEN_BOARD2_DISPLAY_LIST_part_0998, NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0AF8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0E50, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0F00, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_0FA8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_1050, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_1238, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_12E0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_1388, NULL,                                 NULL }
};

DisplayLists gDamienBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_31F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3238, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3278, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_32B8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_32F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3338, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3378, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_33B8, DAMIEN_BOARD2_DISPLAY_LIST_part_33F8, NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3438, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3478, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_34B8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_34F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3538, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_3578, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_35B8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD2_DISPLAY_LIST_part_35F8, NULL,                                 NULL }
};

DisplayLists gDamienBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0000, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_00B8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0170, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0328, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0488, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0540, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_06F8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0858, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0AB0, DAMIEN_BOARD3_DISPLAY_LIST_part_0CF8, NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0DF0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0EA0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0F48, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_0FF0, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_1188, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_1230, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_12D8, NULL,                                 NULL }
};

DisplayLists gDamienBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_2F48, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_2F88, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_2FC8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3008, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3048, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3088, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_30C8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3108, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3148, DAMIEN_BOARD3_DISPLAY_LIST_part_3188, NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_31C8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3208, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3248, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3288, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_32C8, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3308, NULL,                                 NULL },
    { 0x00000000, DAMIEN_BOARD3_DISPLAY_LIST_part_3348, NULL,                                 NULL }
};

DisplayLists gPenBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0068, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_00D0, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0160, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0290, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0320, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_03E8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0588, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0880, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0938, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_09F0, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0AC8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0B70, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0C28, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0D00, NULL, NULL }
};

DisplayLists gPenBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0068, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2038, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2078, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_20B8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_20F8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2138, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2178, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_21B8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_21E8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2228, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2268, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_22A8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_22E8, NULL, NULL },
    { 0x00000000, PEN_BOARD0_DISPLAY_LIST_part_2328, NULL, NULL }
};

DisplayLists gPenBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0000, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0068, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_00D0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0160, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0228, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0290, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0320, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_03E8, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0588, PEN_BOARD1_DISPLAY_LIST_part_0860, NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0A10, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0AC8, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0B80, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0C58, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0D00, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0DB8, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0E90, NULL,                              NULL }
};

DisplayLists gPenBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0000, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0068, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_23B0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_23F0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_0228, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_2430, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_2470, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_24B0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_24F0, PEN_BOARD1_DISPLAY_LIST_part_2530, NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_2570, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_25A0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_25E0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_2620, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_2660, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_26A0, NULL,                              NULL },
    { 0x00000000, PEN_BOARD1_DISPLAY_LIST_part_26E0, NULL,                              NULL }
};

DisplayLists gPenBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0068, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_00D0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0160, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0290, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0320, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_03E8, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0588, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0848, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0900, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_09B8, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0A90, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0B38, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0BF0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0CC8, NULL, NULL }
};

DisplayLists gPenBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0068, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_1FB0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_1FF0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_2030, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_2070, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_20B0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_20F0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_2130, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_2160, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_21A0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_21E0, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_2220, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_2260, NULL, NULL },
    { 0x00000000, PEN_BOARD2_DISPLAY_LIST_part_22A0, NULL, NULL }
};

DisplayLists gPenBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0068, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_00D0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0160, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0290, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0320, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_03E8, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0588, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0838, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_08F0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_09A8, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0A80, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0B28, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0BE0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0CB8, NULL, NULL }
};

DisplayLists gPenBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0068, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_1F80, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_1FC0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_0228, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2000, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2040, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2080, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_20C0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2100, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2130, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2170, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_21B0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_21F0, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2230, NULL, NULL },
    { 0x00000000, PEN_BOARD3_DISPLAY_LIST_part_2270, NULL, NULL }
};

DisplayLists gDogBoard0PrimaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0000, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0070, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0150, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0208, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0370, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0450, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0508, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0670, DOG_BOARD0_DISPLAY_LIST_part_06E8, NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0888, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0AD8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0C90, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0D30, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0DD8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0EB8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_0F58, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_1000, NULL,                              NULL }
};

DisplayLists gDogBoard0SecondaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_29A8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_29E8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2A28, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2A68, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2AA8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2AE8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2B28, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2B68, DOG_BOARD0_DISPLAY_LIST_part_2BA8, NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2BE8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2C28, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2C68, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2CA8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2CE8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2D28, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2D68, NULL,                              NULL },
    { 0x00000000, DOG_BOARD0_DISPLAY_LIST_part_2DA8, NULL,                              NULL }
};

DisplayLists gDogBoard1PrimaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_18D0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1940, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1A18, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1AC8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1C20, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1CF8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1DA8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_1F00, DOG_BOARD1_DISPLAY_LIST_part_1F78, NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_20E0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_22D0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2450, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_24F0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2598, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2640, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_26E0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2788, NULL,                              NULL }
};

DisplayLists gDogBoard1SecondaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2848, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2888, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_28C8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2908, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2948, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2988, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_29C8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2A08, DOG_BOARD1_DISPLAY_LIST_part_2A48, NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2A88, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2AC8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2B08, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2B48, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2B88, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2BC8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2C08, NULL,                              NULL },
    { 0x00000000, DOG_BOARD1_DISPLAY_LIST_part_2C48, NULL,                              NULL }
};

DisplayLists gDogBoard2PrimaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0000, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0070, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0148, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_01F8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_02C0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0398, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0448, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0510, DOG_BOARD2_DISPLAY_LIST_part_06A8, NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0790, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0990, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0AE0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0B80, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0C28, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0CC8, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0D68, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_0E10, NULL,                              NULL }
};

DisplayLists gDogBoard2SecondaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2630, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2670, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_26B0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_26F0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2730, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2770, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_27B0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_27F0, DOG_BOARD2_DISPLAY_LIST_part_2830, NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2870, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_28B0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_28F0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2930, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2970, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_29B0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_29F0, NULL,                              NULL },
    { 0x00000000, DOG_BOARD2_DISPLAY_LIST_part_2A30, NULL,                              NULL }
};

DisplayLists gDogBoard3PrimaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0070, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_01E0, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0320, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0458, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_05C8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0708, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0840, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0A10, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0C48, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0DC8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0E68, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_0F10, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_1048, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_10E8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_1190, NULL, NULL }
};

DisplayLists gDogBoard3SecondaryPartDisplayLists[16] = {
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_2F28, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_2F68, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_2FA8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_2FE8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_3028, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_3068, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_30A8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_30E8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_3128, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_3168, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_31A8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_31E8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_3228, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_3268, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_32A8, NULL, NULL },
    { 0x00000000, DOG_BOARD3_DISPLAY_LIST_part_32E8, NULL, NULL }
};

DisplayLists gDinosBoardPrimaryPartDisplayLists[16] = {
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0000, DINOS_BOARD_DISPLAY_LIST_part_0178, NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0290, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0348, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0400, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0568, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0620, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_06D8, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0840, DINOS_BOARD_DISPLAY_LIST_part_0A98, NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0BA0, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0C78, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0D30, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0E98, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_0F50, NULL,                               NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_10B8, DINOS_BOARD_DISPLAY_LIST_part_1180, NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_1278, DINOS_BOARD_DISPLAY_LIST_part_1340, NULL },
    { 0x00000000, DINOS_BOARD_DISPLAY_LIST_part_1428, NULL,                               NULL }
};

DisplayLists gSnowBoardPrimaryPartDisplayLists[3] = {
    { 0x00000000, SNOW4_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0x00000000, SNOW4_DISPLAY_LIST_part_01E8, NULL, NULL },
    { 0x00000000, SNOW4_DISPLAY_LIST_part_03D0, NULL, NULL }
};

DisplayLists gRoboBoardPrimaryPartDisplayLists[24] = {
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_0000, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_03E0, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_0598, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_0778, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_0930, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_0B10, ROBO_BOARD_DISPLAY_LIST_part_10F8, NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1278, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1430, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_15D0, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1788, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1988, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1A60, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1B38, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_1F10, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_20C8, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_22A8, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_2460, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_2640, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_2BB8, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_2D70, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_2F10, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_30C8, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_32C8, NULL,                              NULL },
    { 0x00000000, ROBO_BOARD_DISPLAY_LIST_part_33A0, NULL,                              NULL }
};

AssetMeta D_8009A550_9B150[] = {
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
    { (void *)0x00000000, (void *)0x01000000 },
    { (void *)0x00000000, (void *)0x00000000 },
};

void *loadAssetByIndex_94F90(s16 groupIndex, s16 pairIndex) {
    AssetMeta *assetArray = gCharacterBoardDisplayListTables[groupIndex];
    return loadUncompressedData(assetArray[pairIndex].start, assetArray[pairIndex].end);
}

void *loadAssetByIndex_95200(s16 groupIndex, s16 pairIndex) {
    CompressedAssetMeta *assetArray = gCharacterBoardCompressedDataTables[groupIndex];
    return loadCompressedData(
        assetArray[pairIndex].start,
        assetArray[pairIndex].end,
        assetArray[pairIndex].uncompressedSize
    );
}

void *loadAssetByIndex_953E0(s16 index) {
    if (index == 0xA) {
        return NULL;
    } else {
        return loadCompressedData(
            gCharacterRaceAnimationAssets[index].start,
            gCharacterRaceAnimationAssets[index].end,
            gCharacterRaceAnimationAssets[index].uncompressedSize
        );
    }
}

DisplayLists *loadAssetByIndex_95380(s16 groupIndex, s16 pairIndex) {
    DisplayLists **group = gCharacterBoardPartDisplayListTables[groupIndex];
    return group[pairIndex];
}

DisplayLists *loadAssetByIndex_953B0(s16 groupIndex, s16 pairIndex) {
    DisplayLists **group = gCharacterBoardSecondaryPartDisplayListTables[groupIndex];
    return group[pairIndex];
}

void *load_3ECE40(void) {
    return loadCompressedData(&ITEM_SPRITE_METADATA_ROM_START, &ITEM_SPRITE_METADATA_ROM_END, 0xA518);
}

void *loadAssetByIndex_95470(s32 index) {
    return loadCompressedData(
        gCharacterFaceIconAssets[index].start,
        gCharacterFaceIconAssets[index].end,
        gCharacterFaceIconAssets[index].uncompressedSize
    );
}

void *loadAssetByIndex_95500(s16 index) {
    return loadUncompressedData(gCostumeSlotDisplayListAssets[index].start, gCostumeSlotDisplayListAssets[index].end);
}

void *loadAssetByIndex_95590(s16 index) {
    return loadCompressedData(
        gCostumeSlotCompressedDataAssets[index].start,
        gCostumeSlotCompressedDataAssets[index].end,
        gCostumeSlotCompressedDataAssets[index].uncompressedSize
    );
}

void *loadAssetByIndex_95668(s16 index) {
    s16 clamped_index;

    if (index == 0x10) {
        return 0;
    }

    clamped_index = index;
    if (index >= 0x10) {
        clamped_index = 0;
    }

    return loadCompressedData(
        gCostumePaletteAssets[clamped_index].start,
        gCostumePaletteAssets[clamped_index].end,
        gCostumePaletteAssets[clamped_index].uncompressedSize
    );
}

void *loadAssetByIndex_95728(s16 index) {
    return gCostumeSlotDisplayListTable[index];
}

void *loadBossHudAssetByRaceType(s32 index) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->raceType == RACE_TYPE_BOSS_JUNGLE) {
        return loadCompressedData(&CRAZY_JUNGLE_BOSS_HUD_ASSET_ROM_START, &CRAZY_JUNGLE_BOSS_HUD_ASSET_ROM_END, 0x250);
    } else if (state->raceType == RACE_TYPE_BOSS_JINGLE) {
        return loadCompressedData(&JINGLE_TOWN_BOSS_HUD_ASSET_ROM_START, &JINGLE_TOWN_BOSS_HUD_ASSET_ROM_END, 0x260);
    } else if (state->raceType == RACE_TYPE_BOSS_ICE) {
        return loadCompressedData(&ICE_LAND_BOSS_HUD_ASSET_ROM_START, &ICE_LAND_BOSS_HUD_ASSET_ROM_END, 0x270);
    } else {
        return loadCompressedData(
            gBossHudAssetTable[index].start,
            gBossHudAssetTable[index].end,
            gBossHudAssetTable[index].uncompressedSize
        );
    }
}

s32 getBossHudAssetSize(s32 index) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->raceType == RACE_TYPE_BOSS_JUNGLE) {
        return 0x250;
    }

    if (state->raceType == RACE_TYPE_BOSS_JINGLE) {
        return 0x260;
    }

    if (state->raceType == RACE_TYPE_BOSS_ICE) {
        return 0x270;
    }

    return gBossHudAssetTable[index].uncompressedSize;
}

void *loadAssetByIndex_5E990(s32 index) {
    return loadCompressedData(
        gGhostInputRecordAssets[index].start,
        gGhostInputRecordAssets[index].end,
        gGhostInputRecordAssets[index].uncompressedSize
    );
}

MemoryAllocatorNode *getAssetDataDirect(s16 groupIndex, s16 entityIndex) {
    return loadAssetDataByMode(groupIndex, entityIndex, MODE_DIRECT_FETCH);
}

MemoryAllocatorNode *loadAssetDataDMA(s16 groupIndex, s16 entityIndex) {
    return loadAssetDataByMode(groupIndex, entityIndex, MODE_DMA);
}

MemoryAllocatorNode *loadAssetDataQueuedDMA(s16 groupIndex, s16 entityIndex) {
    return loadAssetDataByMode(groupIndex, entityIndex, MODE_QUEUED_DMA);
}
