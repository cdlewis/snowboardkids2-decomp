#include "19E80.h"
#include "6E840.h"
#include "common.h"

extern s32 gFontTextureData;
extern s32 gFontPaletteBase;

void func_800680F0_68CF0(void* arg0) {
    func_80019280_19E80_return sp10;

    func_80019280_19E80(arg0, 0, &sp10);
    gFontTextureData = (s32)sp10.unk0;
    gFontPaletteBase = sp10.unk4;
}

INCLUDE_ASM("asm/nonmatchings/68CF0", func_80068128_68D28);

typedef struct {
    u16 x;
    u16 y;
    u16 palette;
    u8* string;
} TextData;

extern void func_80068128_68D28(TextData*);

void func_800688D4_694D4(s16 x, s16 y, s16 palette, u8* target_string, s32 arg4, s32 arg5) {
    s32 required_size;
    s32 length = 0;
    u8* string_block;
    TextData* string_meta_block;
    u8* current_character;

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
        u8* i = string_block;
        if (required_size > 0) {
            u8* j = target_string;
            do {
                *i++ = *j++;
            } while ((s32)i < length + (s32)string_block);
        }

        string_meta_block = (TextData*)advanceLinearAlloc(0xC);
        if (string_meta_block != NULL) {
            string_meta_block->x = x;
            string_meta_block->y = y;
            string_meta_block->palette = palette;
            string_meta_block->string = string_block;
            debugEnqueueCallback(arg4, arg5, &func_80068128_68D28, string_meta_block);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/68CF0", func_800689C0_695C0);
