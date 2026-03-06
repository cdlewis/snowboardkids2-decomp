#include "common.h"
#include "graphics/graphics.h"

void initMenuCameraNode(ViewportNode *node, s32 slotIndex, s32 priority, s32 isSecondary);
void initSplitScreen2P(ViewportNode *viewports, s32 baseSlotIndex, u8 priority, u8 isSecondary);
void initSplitScreen3P4P(ViewportNode *viewports, s32 baseSlotIndex, u8 priority, u8 isSecondary);
