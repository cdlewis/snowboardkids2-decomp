#include "text/font_assets.h"

#include "common.h"
#include "data/data_table.h"
#include "graphics/graphics.h"

#include "gbi.h"

extern s32 gFontTextureData;
extern s32 gFontPaletteBase;
extern s8 gCurrentPaletteId;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;
extern s16 gTextureEnabled[];
extern TextClipAndOffsetData gTextClipAndOffsetData;
extern u16 gDefaultFontPalette[];

Gfx gFontDisplayListSetup[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetCombineLERP(1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0),
    gsDPSetRenderMode(G_RM_AA_TEX_TERR, G_RM_AA_TEX_TERR2),
    gsSPEndDisplayList(),
};

void loadFontAssetsFromDataTable(DataTable_19E80 *dataTable) {
    OutputStruct_19E80 result;

    getTableEntryByU16Index(dataTable, 0, &result);
    gFontTextureData = (s32)result.data_ptr;
    gFontPaletteBase = (s32)result.index_ptr;
}

void renderTextPalette(TextData *arg0) {
    s16 *temp_t2;
    u16 y_pos;
    s16 *font_data;
    u8 char_index;
    u16 x_pos;
    u8 ch;
    u8 *str_ptr;
    s32 i;

    if (gTextureEnabled[0] != 0) {
        gSPTexture(gRegionAllocPtr++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
    }

    x_pos = arg0->x;
    y_pos = arg0->y;

    str_ptr = arg0->string;
    if (*arg0->string == 0) {
        return;
    }

    if (gGraphicsMode != 1) {
        gGraphicsMode = 1;
        gCurrentPaletteId = -1;

        gSPDisplayList(gRegionAllocPtr++, gFontDisplayListSetup);

        gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, gFontTextureData);

        gDPSetTile(
            gRegionAllocPtr++,
            G_IM_FMT_CI,
            G_IM_SIZ_16b,
            0,
            0x0000,
            G_TX_LOADTILE,
            0,
            G_TX_NOMIRROR | G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOMIRROR | G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOLOD
        );

        gDPLoadSync(gRegionAllocPtr++);

        gDPLoadBlock(gRegionAllocPtr++, G_TX_LOADTILE, 0, 0, 1023, 512);

        gDPPipeSync(gRegionAllocPtr++);

        gDPSetTile(
            gRegionAllocPtr++,
            G_IM_FMT_CI,
            G_IM_SIZ_4b,
            4,
            0x0000,
            G_TX_RENDERTILE,
            0,
            G_TX_NOMIRROR | G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOMIRROR | G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOLOD
        );

        gDPSetTileSize(gRegionAllocPtr++, G_TX_RENDERTILE, 0, 0, 0x00FC, 0x00FC);
    }

    if (gCurrentPaletteId != arg0->palette) {
        gCurrentPaletteId = arg0->palette;
        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (arg0->palette << 5));
    }

    str_ptr = arg0->string;
    temp_t2 -= 2;
    while (*str_ptr != '\0') {
        if (*str_ptr < 0x20) {
            if (*str_ptr == '\n') {
                x_pos = arg0->x;
                y_pos += 8;
            }

            if (*str_ptr == 1) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (0x20 * 1));
            }

            if (*str_ptr == 2) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (0x20 * 2));
            }

            if (*str_ptr == 3) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (0x20 * 3));
            }

            if (*str_ptr == 4) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (0x20 * 4));
            }

            if (*str_ptr == 7) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (0x20 * 7));
            }
        } else {
            char_index = *str_ptr - 0x20;
            if (char_index > 0 && char_index < 0x40) {
                s16 char_x = x_pos + gTextureEnabled[-2];
                s16 char_y = y_pos + gTextureEnabled[-1];
                s16 char_right = char_x + 8;
                s16 char_bottom = char_y + 8;
                s16 clip_offset_y = 0;
                s16 clip_offset_x = 0;

                if (char_x < gTextureEnabled[-6]) {
                    clip_offset_x = gTextureEnabled[-6] - char_x;
                    char_x = gTextureEnabled[-6];
                }

                if (char_y < gTextureEnabled[-5]) {
                    clip_offset_y = gTextureEnabled[-5] - char_y;
                    char_y = gTextureEnabled[-5];
                }

                if (gTextureEnabled[-4] >= char_x && gTextureEnabled[-3] >= char_y && char_x < char_right &&
                    char_y < char_bottom) {
                    s16 s = (clip_offset_x + (char_index & 7) * 8) << 5;
                    s16 t = clip_offset_y + (char_index & 0x38);

                    gSPTextureRectangle(
                        gRegionAllocPtr++,
                        char_x << 2,
                        char_y << 2,
                        char_right << 2,
                        char_bottom << 2,
                        G_TX_RENDERTILE,
                        s,
                        t << 5 & 0xffe0,
                        0x0400,
                        0x0400
                    );
                }
            }

            x_pos += 8;
        }

        str_ptr++;
    }
}

void enqueueTextRender(s16 x, s16 y, s16 palette, u8 *target_string, s32 arg4, s32 arg5) {
    s32 required_size;
    s32 length = 0;
    u8 *string_block;
    TextData *string_meta_block;
    u8 *current_character;

    if (*target_string != 0) {
        current_character = target_string;
        while (*current_character != 0) {
            current_character++;
            length++;
        }
    }

    length++;
    required_size = length;
    string_block = advanceLinearAlloc(required_size);
    if (string_block != NULL) {
        // copy string from target_string to newly allocated block
        u8 *i = string_block;
        if (required_size > 0) {
            u8 *j = target_string;
            do {
                *i++ = *j++;
            } while ((s32)i < length + (s32)string_block);
        }

        string_meta_block = (TextData *)advanceLinearAlloc(0xC);
        if (string_meta_block != NULL) {
            string_meta_block->x = x;
            string_meta_block->y = y;
            string_meta_block->palette = palette;
            string_meta_block->string = string_block;
            debugEnqueueCallback(arg4, arg5, &renderTextPalette, string_meta_block);
        }
    }
}

void renderTextColored(ColoredTextRenderArg *arg0) {
    u16 sp6;
    u8 charIndex;
    u16 var_s7;
    u8 *str;

    if (gTextureEnabled[0]) {
        gSPTexture(gRegionAllocPtr++, 0x8000, 0x8000, 0, 0, G_ON);
    }

    var_s7 = arg0->x;
    sp6 = arg0->y;
    str = arg0->string;
    if (*str == 0) {
        return;
    }

    if (gGraphicsMode != 1) {
        gGraphicsMode = 1;
        gCurrentPaletteId = -1;
        gSPDisplayList(gRegionAllocPtr++, gFontDisplayListSetup);
        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            (u32)gFontTextureData,
            G_IM_FMT_CI,
            64,
            64,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            0,
            0,
            0,
            0
        );
    }

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, arg0->shade, arg0->shade, arg0->shade, 0xFF);

    if (gCurrentPaletteId != arg0->palette) {
        gCurrentPaletteId = arg0->palette;
        if (arg0->palette == 0xFF) {
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gDefaultFontPalette);
        } else {
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + (arg0->palette << 5));
        }
    }

    for (str = arg0->string; *str != 0; str++) {
        if (*str < 0x20U) {
            if (*str == 0xA) {
                var_s7 = arg0->x;
                sp6 += 8;
            }
            if (*str == 1) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + 0x20);
            }
            if (*str == 2) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + 0x40);
            }
            if (*str == 3) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + 0x60);
            }
            if (*str == 4) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + 0x80);
            }
            if (*str == 7) {
                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gFontPaletteBase + 0xE0);
            }
        } else {
            charIndex = *str - 0x20;
            if (charIndex != 0 && charIndex < 0x40) {
                s16 clipOffsetY = 0;
                s16 clipOffsetX = 0;
                s16 left = var_s7 + gTextClipAndOffsetData.offsetX;
                s16 top = sp6 + gTextClipAndOffsetData.offsetY;
                s16 xh = left + 8;
                s16 yh = top + 8;
                if (left < gTextClipAndOffsetData.clipLeft) {
                    clipOffsetX = gTextClipAndOffsetData.clipLeft - left;
                    left = gTextClipAndOffsetData.clipLeft;
                }
                if (gTextClipAndOffsetData.clipTop > top) {
                    clipOffsetY = gTextClipAndOffsetData.clipTop - top;
                    top = gTextClipAndOffsetData.clipTop;
                }
                if (gTextClipAndOffsetData.clipRight >= left &&
                    ((gTextClipAndOffsetData.clipBottom < top) ^ 1 && left < xh) && top < yh) {
                    s16 s = (clipOffsetX + ((charIndex & 7) * 8)) << 5;
                    s16 t = (clipOffsetY + (charIndex & 0x38));
                    gSPTextureRectangle(
                        gRegionAllocPtr++,
                        left * 4,
                        top * 4,
                        xh * 4,
                        yh * 4,
                        G_TX_RENDERTILE,
                        s,
                        (t << 5) & 0xFFE0,
                        0x400,
                        0x400
                    );
                }
            }
            var_s7 += 8;
        }
    }

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetCombineLERP(gRegionAllocPtr++, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0);
    gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
}
