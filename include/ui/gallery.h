#pragma once

#include "common.h"

typedef struct FD98_struct FD98_struct;
typedef struct GalleryMenuState GalleryMenuState;

void initGalleryMenu(void);
void initGalleryViewer(FD98_struct *state);
void onGalleryMenuExit(void);
void onGalleryViewerCleanup(void);
void updateGalleryViewer(GalleryMenuState *state);
