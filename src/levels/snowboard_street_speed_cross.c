#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "levels/snowboard_street.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"

static void updateSpeedCrossRotatingSky(SnowboardStreetRotatingSky *sky);
static void cleanupSpeedCrossRotatingSky(SnowboardStreetRotatingSky *sky);

void initSpeedCrossRotatingSky(SnowboardStreetRotatingSky *sky) {
    GameState *gameState;
    DisplayListObject *displayObject;

    gameState = (GameState *)getCurrentAllocation();
    displayObject = &sky->displayObject;
    displayObject->displayLists = &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists1;
    displayObject->segment1 = loadUncompressedAssetByIndex(0xC);
    displayObject->segment2 = loadCompressedSegment2AssetByIndex(0xC);
    displayObject->transform.translation.x = 0x25990000;
    displayObject->transform.translation.y = 0x1A2B0000;
    displayObject->segment3 = NULL;
    displayObject->transform.translation.z = 0xF7A30000;
    setCleanupCallback(cleanupSpeedCrossRotatingSky);
    setCallback(updateSpeedCrossRotatingSky);
}

static void updateSpeedCrossRotatingSky(SnowboardStreetRotatingSky *sky) {
    s32 i;

    sky->rotationAngle -= 0x20;
    createYRotationMatrix(&sky->displayObject.transform, sky->rotationAngle);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &sky->displayObject);
    }
}

static void cleanupSpeedCrossRotatingSky(SnowboardStreetRotatingSky *sky) {
    DisplayListObject *displayObject = &sky->displayObject;

    displayObject->segment1 = freeNodeMemory(displayObject->segment1);
    displayObject->segment2 = freeNodeMemory(displayObject->segment2);
}
