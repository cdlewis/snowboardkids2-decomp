#include "A9A40.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800B9B90_A9A40);

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800B9EF0_A9DA0);

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800BA4B8_AA368);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 unk3;
} PlayerUnk28Entry;

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
    u8 pad16[0x2];
    u16 unk18;
    u8 pad1A[0xA];
} Section3Entry;

typedef struct {
    s32 valX;
    s32 _pad1;
    s32 valZ;
    s32 _pad2;
} StackVars_A9A40;

extern u8 D_800BADD0_AAC80[];

#define SEC3(gs) ((Section3Entry *)((gs)->gameData.section3Data))
#define SEC1(gs) ((Vec3s *)((gs)->gameData.section1Data))

s8 func_800BA694_AA544(Player *player) {
    GameState *gs;
    volatile StackVars_A9A40 sv;
    s32 diffX;
    s32 diffZ;
    s32 distSq;
    s32 dist;
    s32 normalizedX;
    s32 normalizedZ;
    s32 posX;
    s32 posZ;
    s64 product;
    u16 idx14;
    u16 idx18;

    gs = getCurrentAllocation();

    if (player->unk28 != NULL && ((PlayerUnk28Entry *)player->unk28)[player->unkB94].unk3 != 0) {
        idx14 = SEC3(gs)[player->unkB94].unk14;
        idx18 = SEC3(gs)[player->unkB94].unk18;
        diffX = SEC1(gs)[idx14].x - SEC1(gs)[idx18].x;
        sv.valX = diffX;

        idx14 = SEC3(gs)[player->unkB94].unk14;
        distSq = diffX * diffX;
        idx18 = SEC3(gs)[player->unkB94].unk18;
        diffZ = SEC1(gs)[idx14].z - SEC1(gs)[idx18].z;
        sv.valZ = diffZ;
        distSq += diffZ * diffZ;

        dist = isqrt64(distSq);
        normalizedX = (sv.valX << 13) / dist;
        normalizedZ = (sv.valZ << 13) / dist;

        idx14 = SEC3(gs)[player->unkB94].unk14;
        posX = player->worldPos.x - (SEC1(gs)[idx14].x << 16);
        sv.valX = posX;

        idx14 = SEC3(gs)[player->unkB94].unk14;
        posZ = player->worldPos.z - (SEC1(gs)[idx14].z << 16);
        sv.valZ = posZ;

        product = (((s64)(-((s16)normalizedZ))) * posX) + (((s64)((s16)normalizedX)) * posZ);
        dist = -((s32)(product / 0x2000));
        if (dist < (player->unkAA8 * 6)) {
            return ((PlayerUnk28Entry *)player->unk28)[player->unkB94].unk3;
        }
    }

    if (!(player->unkBDE & 8)) {
        player->unkBE5 = 0;
    }

    if (gs->unk86 != 0) {
        if (player->unkBE5 == 0 && (player->unkBDE & 8)) {
            if ((randA() & 0xFF) >= 0xC0) {
                return 0;
            }
            player->unkBE5 = 1;
            return 8;
        }
        return 0;
    }

    if (gs->unk7A != 9) {
        if (player->unkBE5 == 0 && (player->unkBDE & 8)) {
            if ((randA() & 0xFF) < D_800BADD0_AAC80[gs->memoryPoolId]) {
                player->unkBE5 = 1;
                return 8;
            }
            if (player->unkBB9 >= 6) {
                player->unkBE5 = 1;
                return 8;
            }
        }
    }

    return 0;
}
