#include "data/asset_metadata.h"
#include "assets.h"
#include "common.h"
#include "gamestate.h"
#include "gbi.h"
#include "generated/model_gfx.h"
#include "generated/modelpayload_asset_sizes.h"
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
    { &SLASH_BOARD0_COMPRESSED_DATA_ROM_START,
     &SLASH_BOARD0_COMPRESSED_DATA_ROM_END,
     SLASH_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &SLASH_BOARD1_COMPRESSED_DATA_ROM_START,
     &SLASH_BOARD1_COMPRESSED_DATA_ROM_END,
     SLASH_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &SLASH_BOARD2_COMPRESSED_DATA_ROM_START,
     &SLASH_BOARD2_COMPRESSED_DATA_ROM_END,
     SLASH_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &SLASH_BOARD3_COMPRESSED_DATA_ROM_START,
     &SLASH_BOARD3_COMPRESSED_DATA_ROM_END,
     SLASH_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gNancyBoardCompressedDataAssets[] = {
    { &NANCY_BOARD0_COMPRESSED_DATA_ROM_START,
     &NANCY_BOARD0_COMPRESSED_DATA_ROM_END,
     NANCY_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &NANCY_BOARD1_COMPRESSED_DATA_ROM_START,
     &NANCY_BOARD1_COMPRESSED_DATA_ROM_END,
     NANCY_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &NANCY_BOARD2_COMPRESSED_DATA_ROM_START,
     &NANCY_BOARD2_COMPRESSED_DATA_ROM_END,
     NANCY_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &NANCY_BOARD3_COMPRESSED_DATA_ROM_START,
     &NANCY_BOARD3_COMPRESSED_DATA_ROM_END,
     NANCY_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gJamBoardCompressedDataAssets[] = {
    { &JAM_BOARD0_COMPRESSED_DATA_ROM_START,
     &JAM_BOARD0_COMPRESSED_DATA_ROM_END,
     JAM_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &JAM_BOARD1_COMPRESSED_DATA_ROM_START,
     &JAM_BOARD1_COMPRESSED_DATA_ROM_END,
     JAM_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &JAM_BOARD2_COMPRESSED_DATA_ROM_START,
     &JAM_BOARD2_COMPRESSED_DATA_ROM_END,
     JAM_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &JAM_BOARD3_COMPRESSED_DATA_ROM_START,
     &JAM_BOARD3_COMPRESSED_DATA_ROM_END,
     JAM_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gLindaBoardCompressedDataAssets[] = {
    { &LINDA_BOARD0_COMPRESSED_DATA_ROM_START,
     &LINDA_BOARD0_COMPRESSED_DATA_ROM_END,
     LINDA_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &LINDA_BOARD1_COMPRESSED_DATA_ROM_START,
     &LINDA_BOARD1_COMPRESSED_DATA_ROM_END,
     LINDA_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &LINDA_BOARD2_COMPRESSED_DATA_ROM_START,
     &LINDA_BOARD2_COMPRESSED_DATA_ROM_END,
     LINDA_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &LINDA_BOARD3_COMPRESSED_DATA_ROM_START,
     &LINDA_BOARD3_COMPRESSED_DATA_ROM_END,
     LINDA_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gTommyBoardCompressedDataAssets[] = {
    { &TOMMY_BOARD0_COMPRESSED_DATA_ROM_START,
     &TOMMY_BOARD0_COMPRESSED_DATA_ROM_END,
     TOMMY_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &TOMMY_BOARD1_COMPRESSED_DATA_ROM_START,
     &TOMMY_BOARD1_COMPRESSED_DATA_ROM_END,
     TOMMY_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &TOMMY_BOARD2_COMPRESSED_DATA_ROM_START,
     &TOMMY_BOARD2_COMPRESSED_DATA_ROM_END,
     TOMMY_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &TOMMY_BOARD3_COMPRESSED_DATA_ROM_START,
     &TOMMY_BOARD3_COMPRESSED_DATA_ROM_END,
     TOMMY_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gWendyBoardCompressedDataAssets[] = {
    { &WENDY_BOARD0_COMPRESSED_DATA_ROM_START,
     &WENDY_BOARD0_COMPRESSED_DATA_ROM_END,
     WENDY_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &WENDY_BOARD1_COMPRESSED_DATA_ROM_START,
     &WENDY_BOARD1_COMPRESSED_DATA_ROM_END,
     WENDY_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &WENDY_BOARD2_COMPRESSED_DATA_ROM_START,
     &WENDY_BOARD2_COMPRESSED_DATA_ROM_END,
     WENDY_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &WENDY_BOARD3_COMPRESSED_DATA_ROM_START,
     &WENDY_BOARD3_COMPRESSED_DATA_ROM_END,
     WENDY_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gDamienBoardCompressedDataAssets[] = {
    { &DAMIEN_BOARD0_COMPRESSED_DATA_ROM_START,
     &DAMIEN_BOARD0_COMPRESSED_DATA_ROM_END,
     DAMIEN_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DAMIEN_BOARD1_COMPRESSED_DATA_ROM_START,
     &DAMIEN_BOARD1_COMPRESSED_DATA_ROM_END,
     DAMIEN_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DAMIEN_BOARD2_COMPRESSED_DATA_ROM_START,
     &DAMIEN_BOARD2_COMPRESSED_DATA_ROM_END,
     DAMIEN_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DAMIEN_BOARD3_COMPRESSED_DATA_ROM_START,
     &DAMIEN_BOARD3_COMPRESSED_DATA_ROM_END,
     DAMIEN_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gPenBoardCompressedDataAssets[] = {
    { &PEN_BOARD0_COMPRESSED_DATA_ROM_START,
     &PEN_BOARD0_COMPRESSED_DATA_ROM_END,
     PEN_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &PEN_BOARD1_COMPRESSED_DATA_ROM_START,
     &PEN_BOARD1_COMPRESSED_DATA_ROM_END,
     PEN_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &PEN_BOARD2_COMPRESSED_DATA_ROM_START,
     &PEN_BOARD2_COMPRESSED_DATA_ROM_END,
     PEN_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &PEN_BOARD3_COMPRESSED_DATA_ROM_START,
     &PEN_BOARD3_COMPRESSED_DATA_ROM_END,
     PEN_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gDogBoardCompressedDataAssets[] = {
    { &DOG_BOARD0_COMPRESSED_DATA_ROM_START,
     &DOG_BOARD0_COMPRESSED_DATA_ROM_END,
     DOG_BOARD0_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DOG_BOARD1_COMPRESSED_DATA_ROM_START,
     &DOG_BOARD1_COMPRESSED_DATA_ROM_END,
     DOG_BOARD1_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DOG_BOARD2_COMPRESSED_DATA_ROM_START,
     &DOG_BOARD2_COMPRESSED_DATA_ROM_END,
     DOG_BOARD2_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DOG_BOARD3_COMPRESSED_DATA_ROM_START,
     &DOG_BOARD3_COMPRESSED_DATA_ROM_END,
     DOG_BOARD3_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gDinosBoardCompressedDataAssets[] = {
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START,
     &DINOS_BOARD_COMPRESSED_DATA_ROM_END,
     DINOS_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START,
     &DINOS_BOARD_COMPRESSED_DATA_ROM_END,
     DINOS_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START,
     &DINOS_BOARD_COMPRESSED_DATA_ROM_END,
     DINOS_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &DINOS_BOARD_COMPRESSED_DATA_ROM_START,
     &DINOS_BOARD_COMPRESSED_DATA_ROM_END,
     DINOS_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gSnowBoardCompressedDataAssets[] = {
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, SNOW4_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, SNOW4_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, SNOW4_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &SNOW4_COMPRESSED_DATA_ROM_START, &SNOW4_COMPRESSED_DATA_ROM_END, SNOW4_COMPRESSED_DATA_DECOMPRESSED_SIZE },
};

CompressedAssetMeta gRoboBoardCompressedDataAssets[] = {
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START,
     &ROBO_BOARD_COMPRESSED_DATA_ROM_END,
     ROBO_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START,
     &ROBO_BOARD_COMPRESSED_DATA_ROM_END,
     ROBO_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START,
     &ROBO_BOARD_COMPRESSED_DATA_ROM_END,
     ROBO_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &ROBO_BOARD_COMPRESSED_DATA_ROM_START,
     &ROBO_BOARD_COMPRESSED_DATA_ROM_END,
     ROBO_BOARD_COMPRESSED_DATA_DECOMPRESSED_SIZE },
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
    { &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_00_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_01_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_01_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_01_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_02_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_02_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_02_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_03_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_03_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_03_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_04_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_04_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_04_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_05_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_05_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_05_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_06_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_06_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_06_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_07_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_07_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_07_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_08_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_08_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_08_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_09_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_09_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_09_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_10_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_10_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_10_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_11_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_11_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_11_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_12_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_12_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_12_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_13_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_13_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_13_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_14_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_14_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_14_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_15_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_15_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_15_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_16_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_16_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_16_COMPRESSED_DATA_DECOMPRESSED_SIZE },
    { &COSTUME_SLOT_17_COMPRESSED_DATA_ROM_START,
     &COSTUME_SLOT_17_COMPRESSED_DATA_ROM_END,
     COSTUME_SLOT_17_COMPRESSED_DATA_DECOMPRESSED_SIZE }
};

CompressedAssetMeta gCostumePaletteAssets[] = {
    { &PALETTE_00_ROM_START, &PALETTE_00_ROM_END, PALETTE_00_DECOMPRESSED_SIZE },
    { &PALETTE_01_ROM_START, &PALETTE_01_ROM_END, PALETTE_01_DECOMPRESSED_SIZE },
    { &PALETTE_02_ROM_START, &PALETTE_02_ROM_END, PALETTE_02_DECOMPRESSED_SIZE },
    { &PALETTE_03_ROM_START, &PALETTE_03_ROM_END, PALETTE_03_DECOMPRESSED_SIZE },
    { &PALETTE_04_ROM_START, &PALETTE_04_ROM_END, PALETTE_04_DECOMPRESSED_SIZE },
    { &PALETTE_05_ROM_START, &PALETTE_05_ROM_END, PALETTE_05_DECOMPRESSED_SIZE },
    { &PALETTE_06_ROM_START, &PALETTE_06_ROM_END, PALETTE_06_DECOMPRESSED_SIZE },
    { &PALETTE_07_ROM_START, &PALETTE_07_ROM_END, PALETTE_07_DECOMPRESSED_SIZE },
    { &PALETTE_08_ROM_START, &PALETTE_08_ROM_END, PALETTE_08_DECOMPRESSED_SIZE },
    { &PALETTE_09_ROM_START, &PALETTE_09_ROM_END, PALETTE_09_DECOMPRESSED_SIZE },
    { &PALETTE_10_ROM_START, &PALETTE_10_ROM_END, PALETTE_10_DECOMPRESSED_SIZE },
    { &PALETTE_11_ROM_START, &PALETTE_11_ROM_END, PALETTE_11_DECOMPRESSED_SIZE },
    { &PALETTE_12_ROM_START, &PALETTE_12_ROM_END, PALETTE_12_DECOMPRESSED_SIZE },
    { &PALETTE_13_ROM_START, &PALETTE_13_ROM_END, PALETTE_13_DECOMPRESSED_SIZE },
    { &PALETTE_14_ROM_START, &PALETTE_14_ROM_END, PALETTE_14_DECOMPRESSED_SIZE },
    { &PALETTE_15_ROM_START, &PALETTE_15_ROM_END, PALETTE_15_DECOMPRESSED_SIZE },
};

DisplayLists *gCostumeSlotDisplayListTable[] = {
    &gCostumeSlotDisplayLists[0],  &gCostumeSlotDisplayLists[1],  &gCostumeSlotDisplayLists[2],
    &gCostumeSlotDisplayLists[3],  &gCostumeSlotDisplayLists[4],  &gCostumeSlotDisplayLists[5],
    &gCostumeSlotDisplayLists[6],  &gCostumeSlotDisplayLists[7],  &gCostumeSlotDisplayLists[8],
    &gCostumeSlotDisplayLists[9],  &gCostumeSlotDisplayLists[10], &gCostumeSlotDisplayLists[11],
    &gCostumeSlotDisplayLists[12], &gCostumeSlotDisplayLists[13], &gCostumeSlotDisplayLists[14],
    &gCostumeSlotDisplayLists[15], &gCostumeSlotDisplayLists[16], &gCostumeSlotDisplayLists[17],
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
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_70D8, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_71B0, NULL },
};

DisplayLists gSunnyMountainFogDisplayLists[] = {
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_7550, NULL, NULL },
};

DisplayLists gSunnyMountainDisplayListTable[] = {
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_7560, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_78E0, NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_7A08, NULL,                                          NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_7C08, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_7F48, NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8028, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_81A8, NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8328, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_84A8, NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8628, NULL,                                          NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8760, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_87E8, NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_88C8, NULL,                                          NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8968, NULL,                                          NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8A08, NULL,                                          NULL },
    { 0, NULL,                                          SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8DA8, NULL },
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_8E88, NULL,                                          NULL },
};

DisplayLists gSunnyMountainScrollingSceneryDisplayLists[] = {
    { 0, SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS_part_9200, NULL, NULL },
};

DisplayLists gTurtleIslandSkyDisplayLists[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_9950, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_9AD8, NULL },
};

DisplayLists gTurtleIslandFogDisplayLists[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_9DF0, NULL, NULL },
};

DisplayLists gTurtleIslandDefaultFogDisplayLists[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A020, NULL, NULL },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists1[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A110, NULL, NULL },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists2[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A1B8, NULL, NULL },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists3[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A240, NULL, NULL },
};

DisplayLists gTurtleIslandScrollingSceneryDisplayLists4[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A330, NULL, NULL },
};

DisplayLists gTurtleIslandDisplayListTable[] = {
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A340, NULL,                                         NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A600, NULL,                                         NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_A8E8, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_AD48, NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_AE48, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_AFE8, NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B0E0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B280, NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B378, NULL,                                         NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B4D0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B558, NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B638, NULL,                                         NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B6D8, NULL,                                         NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B778, NULL,                                         NULL },
    { 0, TURTLE_ISLAND_COURSE_DISPLAY_LISTS_part_B860, NULL,                                         NULL },
};

DisplayLists gJingleTownSkyDisplayLists[] = {
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_A070, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_A1F8, NULL },
};

DisplayLists gJingleTownScrollingSceneryDisplayLists1[] = {
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_A378, NULL, NULL },
};

DisplayLists gJingleTownScrollingSceneryDisplayLists2[] = {
    { 0, NULL, NULL, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_A448 },
};

DisplayLists gJingleTownFogDisplayLists[] = {
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_A718, NULL, NULL },
};

DisplayLists gJingleTownDisplayListTable[] = {
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_A728, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_AC08, NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_AD68, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_AF68, NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_B0C8, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_B410, NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_B4F0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_B6B0, NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_B840, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BA00, NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BB90, NULL,                                       NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BCD8, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BD60, NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BE48, NULL,                                       NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BF00, NULL,                                       NULL },
    { 0, JINGLE_TOWN_COURSE_DISPLAY_LISTS_part_BFB8, NULL,                                       NULL },
};

DisplayLists gJingleTownBossSkyDisplayLists[] = {
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_9CC8, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_9E50, NULL },
};

DisplayLists gJingleTownBossScrollingSceneryDisplayLists1[] = {
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_9FC0, NULL, NULL },
};

DisplayLists gJingleTownBossScrollingSceneryDisplayLists2[] = {
    { 0, NULL, NULL, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_A090 },
};

DisplayLists gJingleTownBossFogDisplayLists[] = {
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_A350, NULL, NULL },
};

DisplayLists gJingleTownBossDisplayListTable[] = {
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_A360, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_A858, NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_A9C8, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_ABD0, NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_AD60, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B0C0, NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B1A0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B338, NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B4E0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B678, NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B820, NULL,                                            NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B958, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_B9E0, NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_BAC8, NULL,                                            NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_BB78, NULL,                                            NULL },
    { 0, JINGLE_TOWN_BOSS_COURSE_DISPLAY_LISTS_part_BC28, NULL,                                            NULL },
};

DisplayLists gWendysHouseSkyDisplayLists[] = {
    { 0,
     WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_A3E0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_A600,
     WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_A6E0 },
};

DisplayLists gWendysHouseFogDisplayLists[] = {
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_A870, NULL, NULL },
};

DisplayLists gWendysHouseScrollingSceneryDisplayLists[] = {
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_A918, NULL, NULL },
};

DisplayLists gWendysHouseDisplayListTable[] = {
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_A928, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_AD28, NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_AE50, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_B050, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_B390, NULL },
    { 0, NULL,                                        WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_B470, NULL },
    { 0, NULL,                                        WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_B688, NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_B8A0, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_B9E8, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_BA70, NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_BB58, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_BC08, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_BCB8, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_BE00, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_BF00, NULL,                                        NULL },
    { 0, WENDYS_HOUSE_COURSE_DISPLAY_LISTS_part_C118, NULL,                                        NULL },
};

DisplayLists gLindasCastleSkyDisplayLists[] = {
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_C0B0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_C230, NULL },
};

DisplayLists gLindasCastleFogDisplayLists[] = {
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_C728, NULL, NULL },
};

DisplayLists gLindasCastleDisplayListTable[] = {
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_C738, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_C8D8, NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_C9C0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_CBD8, NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_CCF0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_CDC0, NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_CEA8, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_CFC0, NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D0A8, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D1C0, NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D2B8, NULL,                                         NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D3E8, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D478, NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D558, NULL,                                         NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D5F8, NULL,                                         NULL },
    { 0, LINDAS_CASTLE_COURSE_DISPLAY_LISTS_part_D698, NULL,                                         NULL },
};

DisplayLists gCrazyJungleSkyDisplayLists[] = {
    { 0,
     CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_7F88, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_80B0,
     CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_8198 },
};

DisplayLists gCrazyJungleFogDisplayLists[] = {
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_83F0, NULL, NULL },
};

DisplayLists gCrazyJungleDisplayListTable[] = {
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_8400, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_86E0, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_8950, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_8D08, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_8DF0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_90A0, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9308, NULL,                                        NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9498, NULL,                                        NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9628, NULL,                                        NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_97C0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9848, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9928, NULL,                                        NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_99C8, NULL,                                        NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9A68, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9BD8, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9D30, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_9F10, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_A068, NULL,                                        NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_A148, NULL,                                        NULL },
};

DisplayLists gCrazyJunglePushZoneDisplayLists[2] = {
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_A228, NULL, NULL },
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_A2C8, NULL, NULL }
};

DisplayLists gCrazyJungleScrollingSceneryDisplayLists1[] = {
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_A7D0, NULL, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_A7E8 },
};

DisplayLists gCrazyJungleScrollingSceneryDisplayLists2[] = {
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_AA70, NULL, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_AA80 },
};

DisplayLists gCrazyJungleScrollingSceneryDisplayLists3[] = {
    { 0, CRAZY_JUNGLE_COURSE_DISPLAY_LISTS_part_AB18, NULL, NULL },
};

DisplayLists gCrazyJungleBossSkyDisplayLists[] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_9300, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_9430, NULL },
};

DisplayLists gCrazyJungleBossFogDisplayLists[] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_9888, NULL, NULL },
};

DisplayLists gCrazyJungleBossDisplayListTable[] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_9898, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_9B70, NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_9DE0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_A190, NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_A278, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_A520, NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_A788, NULL,                                             NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_A918, NULL,                                             NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_AAA8, NULL,                                             NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_AC40, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_ACC8, NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_ADA8, NULL,                                             NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_AE48, NULL,                                             NULL },
};

DisplayLists gCrazyJungleBossPushZoneDisplayLists[2] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_AEE8, NULL, NULL },
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_AF88, NULL, NULL }
};

DisplayLists gCrazyJungleBossScrollingSceneryDisplayLists1[] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_B4B0, NULL, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_B4C8 },
};

DisplayLists gCrazyJungleBossScrollingSceneryDisplayLists2[] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_B790, NULL, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_B7A0 },
};

DisplayLists gCrazyJungleBossScrollingSceneryDisplayLists3[] = {
    { 0, CRAZY_JUNGLE_BOSS_COURSE_DISPLAY_LISTS_part_B838, NULL, NULL },
};

DisplayLists gStarlightHighwaySkyDisplayLists[] = {
    { 0,
     STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_B688, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_B8A0,
     STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_B9A0 },
};

DisplayLists gStarlightHighwayFogDisplayLists[] = {
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_BDC0, NULL, NULL },
};

DisplayLists gStarlightHighwayDisplayListTable[] = {
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_BDD0, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_C028, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_C280, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_C4D8, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_C678, NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_C770, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_C910, NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CA08, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CBB0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CC38, NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CD18, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CDC8, NULL,                                             NULL },
    { 0, NULL,                                             STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CE78, NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_CFE0, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_D128, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_D270, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_D3E8, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_D560, NULL,                                             NULL },
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_D630, NULL,                                             NULL },
};

DisplayLists gDebugDisplayConfig = { 0, NULL, NULL, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_D798 };

DisplayLists gStarlightHighwayScrollingSceneryDisplayLists1[] = {
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_DDF0, NULL, NULL },
};

DisplayLists gStarlightHighwayScrollingSceneryDisplayLists2[] = {
    { 0, NULL, NULL, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_E200 },
};

DisplayLists gStarlightHighwayScrollingSceneryDisplayLists3[] = {
    { 0, STARLIGHT_HIGHWAY_COURSE_DISPLAY_LISTS_part_E558, NULL, NULL },
};

DisplayLists gHauntedHouseSkyDisplayLists[] = {
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_B878, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_BB48, NULL },
};

DisplayLists gHauntedHouseFogDisplayLists[] = {
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_BE90, NULL, NULL },
};

DisplayLists gHauntedHouseDisplayListTable[] = {
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_BEA0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_C530, NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_C648, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_C7D8, NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_C8C0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_CF78, NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D058, NULL,                                         NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D1F0, NULL,                                         NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D388, NULL,                                         NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D4B8, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D5B8, NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D698, NULL,                                         NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D748, NULL,                                         NULL },
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_D7F8, NULL,                                         NULL },
};

DisplayLists gHauntedHousePushZoneDisplayLists[1] = {
    { 0, HAUNTED_HOUSE_COURSE_DISPLAY_LISTS_part_DD30, NULL, NULL }
};

DisplayLists gIceLandSkyDisplayLists[] = {
    { 0,
     ICE_LAND_COURSE_DISPLAY_LISTS_part_8CD8, ICE_LAND_COURSE_DISPLAY_LISTS_part_8E18,
     ICE_LAND_COURSE_DISPLAY_LISTS_part_8EB8 },
};

DisplayLists gIceLandDisplayListTable[] = {
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_8EE0, ICE_LAND_COURSE_DISPLAY_LISTS_part_91A8, NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_9328, ICE_LAND_COURSE_DISPLAY_LISTS_part_9770, NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_9968, ICE_LAND_COURSE_DISPLAY_LISTS_part_9B60, NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_9C58, ICE_LAND_COURSE_DISPLAY_LISTS_part_9D50, NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_9E90, ICE_LAND_COURSE_DISPLAY_LISTS_part_9F88, NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_A0C8, NULL,                                    NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_A228, ICE_LAND_COURSE_DISPLAY_LISTS_part_A2B0, NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_A390, NULL,                                    NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_A430, NULL,                                    NULL },
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_A4D0, NULL,                                    NULL },
};

DisplayLists gIceLandPushZoneDisplayLists[1] = {
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_A678, ICE_LAND_COURSE_DISPLAY_LISTS_part_A718, NULL }
};

DisplayLists gIceLandFogDisplayLists[] = {
    { 0, ICE_LAND_COURSE_DISPLAY_LISTS_part_AAA0, NULL, NULL },
};

DisplayLists gIceLandBossSkyDisplayLists[] = {
    { 0,
     ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_8920, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_8A60,
     ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_8B00 },
};

DisplayLists gIceLandBossDisplayListTable[] = {
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_8B28, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_8DF0, NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_8F78, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_93C0, NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_95B8, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_97C0, NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_98A8, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9990, NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9AC0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9BA8, NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9CD8, NULL,                                         NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9E30, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9EB8, NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_9F98, NULL,                                         NULL },
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_A038, NULL,                                         NULL },
};

DisplayLists gIceLandBossPushZoneDisplayLists[1] = {
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_A0D8, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_A178, NULL }
};

DisplayLists gIceLandBossFogDisplayLists[] = {
    { 0, ICE_LAND_BOSS_COURSE_DISPLAY_LISTS_part_A500, NULL, NULL },
};

DisplayLists gSnowboardStreetSpeedCrossSkyDisplayLists[] = {
    { 0,
     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_D318, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_D4E0,
     NULL },
};

DisplayLists gSnowboardStreetSpeedCrossScrollingSceneryDisplayLists[] = {
    { 0, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_D7A8, NULL, NULL },
};

DisplayLists gSnowboardStreetSpeedCrossFogDisplayLists[] = {
    { 0, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_DA80, NULL, NULL },
};

DisplayLists gSnowboardStreetSpeedCrossDisplayListTable[] = {
    { 0,
     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_DA90,     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_DEA0,
     NULL                                                                                                                               },
    { 0, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_DFA0, NULL,                                                        NULL },
    { 0,
     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_E180,     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_E590,
     NULL                                                                                                                               },
    { 0,
     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_E670,     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_E7D0,
     NULL                                                                                                                               },
    { 0,
     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_E8C8,     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_EA28,
     NULL                                                                                                                               },
    { 0, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_EB20, NULL,                                                        NULL },
    { 0,
     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_EC80,     SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_ED18,
     NULL                                                                                                                               },
    { 0, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_EE00, NULL,                                                        NULL },
    { 0, SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_EEB0, NULL,                                                        NULL },
    { 0, NULL,                                                        SNOWBOARD_STREET_SPEED_CROSS_COURSE_DISPLAY_LISTS_part_EF60, NULL },
};

DisplayLists gSnowboardStreetShotCrossSkyDisplayLists[] = {
    { 0,
     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_CDF0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_CFB0,
     NULL },
};

DisplayLists gSnowboardStreetShotCrossScrollingSceneryDisplayLists[] = {
    { 0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_D270, NULL, NULL },
};

DisplayLists gSnowboardStreetShotCrossFogDisplayLists[] = {
    { 0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_D550, NULL, NULL },
};

DisplayLists gSnowboardStreetShotCrossDisplayListTable[] = {
    { 0,
     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_D560,     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_D960,
     NULL                                                                                                                             },
    { 0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_DA60, NULL,                                                       NULL },
    { 0,
     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_DC40,     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E038,
     NULL                                                                                                                             },
    { 0,
     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E118,     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E278,
     NULL                                                                                                                             },
    { 0,
     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E370,     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E4D0,
     NULL                                                                                                                             },
    { 0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E5C8, NULL,                                                       NULL },
    { 0,
     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E710,     SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E7A8,
     NULL                                                                                                                             },
    { 0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E890, NULL,                                                       NULL },
    { 0, SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_E950, NULL,                                                       NULL },
    { 0, NULL,                                                       SNOWBOARD_STREET_SHOT_CROSS_COURSE_DISPLAY_LISTS_part_EA00, NULL },
};

DisplayLists gXCrossSkyDisplayLists[] = {
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_6DC0, X_CROSS_COURSE_DISPLAY_LISTS_part_6F20, NULL },
};

DisplayLists gXCrossFogDisplayLists[] = {
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_71E8, NULL, NULL },
};

DisplayLists gXCrossDisplayListTable[] = {
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_71F8, X_CROSS_COURSE_DISPLAY_LISTS_part_7488, NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_7588, NULL,                                   NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_77B0, X_CROSS_COURSE_DISPLAY_LISTS_part_79F0, NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_7AD0, NULL,                                   NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_7C58, NULL,                                   NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_7DE0, NULL,                                   NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_7F28, X_CROSS_COURSE_DISPLAY_LISTS_part_7FB8, NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_8098, NULL,                                   NULL },
    { 0, X_CROSS_COURSE_DISPLAY_LISTS_part_8148, NULL,                                   NULL },
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

DisplayLists gCostumeSlotDisplayLists[18] = {
    { 0, COSTUME_SLOT_00_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_01_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_02_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_03_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_04_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_05_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_06_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_07_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_08_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_09_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_10_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_11_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_12_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_13_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_14_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_15_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_16_DISPLAY_LIST_part_0000, NULL, NULL },
    { 0, COSTUME_SLOT_17_DISPLAY_LIST_part_0000, NULL, NULL }
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

DisplayLists *loadAssetByIndex_95728(s16 index) {
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
