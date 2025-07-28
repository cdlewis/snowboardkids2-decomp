#include "68CF0.h"

#include "19E80.h"
#include "6E840.h"
#include "common.h"

extern s32 gFontTextureData;
extern s32 gFontPaletteBase;

void func_800680F0_68CF0(void *arg0) {
    OutputStruct_19E80 result;

    getTableEntryByU16Index(arg0, 0, &result);
    gFontTextureData = (s32)result.data_ptr;
    gFontPaletteBase = (s32)result.index_ptr;
}

INCLUDE_ASM("asm/nonmatchings/68CF0", renderTextPalette);

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

INCLUDE_ASM("asm/nonmatchings/68CF0", renderTextColored);
