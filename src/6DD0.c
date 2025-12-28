#include "90F0.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"

extern void func_8000A1E4_ADE4(void *, u16, s32, s32, s32, s32, s32, s32, s32, s32, s16);

typedef struct {
    u8 _pad[0xC];
    s16 unkC;
    u8 _pad2[0x2];
    struct {
        u8 padding[0x16];
        u16 unk16;
    } *unk10;
    u8 _pad3[0x18];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 _pad4[0x4];
    s8 isDestroyed;
    s8 actionMode;
    s8 unk3E;
    s8 displayEnabled;
    u8 _pad5[0x48];
    s8 unk88;
} func_80006940_inner;

typedef struct {
    func_80006940_inner *owner;
    SpriteAssetState spriteState;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s16 unk68;
    s16 unk6A;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s16 unk84;
    s16 unk86;
    s16 unk88;
    s16 unk8A;
    s8 unk8C;
} ParticleState;

void cleanupTrailingParticle(ParticleState *);
void updateTrailingParticle(ParticleState *);
void cleanupFallingParticle(ParticleState *);
void updateFallingParticle(ParticleState *);
void cleanupDriftingParticle(ParticleState *);
void updateDriftingParticle(ParticleState *);

void initFallingParticle(ParticleState *arg0) {
    s32 rand1;
    s32 rand2;
    u8 rand3;

    loadSpriteAsset(&arg0->spriteState, 2);
    setSpriteAnimation(&arg0->spriteState, 0x10000, 1, -1);

    rand1 = randA();
    rand1 = rand1 & 0xFF;
    rand1 = rand1 << 11;
    rand2 = randA();

    arg0->unk5C = rand1 + 0xFFFC0000;
    arg0->unk60 = 0;
    arg0->unk64 = (((u8)rand2) << 11) + 0xFFFC0000;

    rand3 = randA();
    arg0->unk6A = (rand3 % 3) + 2;

    setCleanupCallback(cleanupFallingParticle);
    setCallback(updateFallingParticle);
}

void updateFallingParticle(ParticleState *arg0) {
    func_80006940_inner *particleOwner;
    s16 lifetime;
    s16 newLifetime;
    void *sprite;

    if (arg0->owner->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    lifetime = arg0->unk68;
    if (lifetime < 0) {
        func_80069CF8_6A8F8();
        return;
    }

    newLifetime = lifetime - 1;
    sprite = &arg0->spriteState;
    arg0->unk68 = newLifetime;
    updateSpriteAnimation(sprite, 0x10000);

    particleOwner = arg0->owner;
    if (particleOwner->unk88 != 0) {
        if (particleOwner->displayEnabled != 0) {
            func_8000A1E4_ADE4(
                sprite,
                particleOwner->unk10->unk16,
                arg0->unk50 + arg0->unk5C,
                arg0->unk54 + arg0->unk60,
                arg0->unk58 + arg0->unk64,
                0x18000,
                0x18000,
                0,
                0,
                0xFF,
                arg0->unk6A
            );
        }
    }

    arg0->unk60 = arg0->unk60 + 0xFFFEB852;
}

void cleanupFallingParticle(ParticleState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}

void initDriftingParticle(ParticleState *arg0) {
    loadSpriteAsset(&arg0->spriteState, 2);
    setSpriteAnimation(&arg0->spriteState, 0x10000, 0, -1);

    arg0->unk50 = 0x10000;
    arg0->unk5C = 0;
    arg0->unk58 = 0;
    arg0->unk54 = 0;
    arg0->unk74 = 0;
    arg0->unk70 = 0;
    arg0->unk6C = 0;
    arg0->unk80 = 0;
    arg0->unk7C = 0;
    arg0->unk78 = 0;
    arg0->unk88 = 0;
    arg0->unk86 = 0;
    arg0->unk84 = 0;
    arg0->unk8A = 0;
    arg0->unk8C = 0;

    setCleanupCallback(cleanupDriftingParticle);
    setCallback(updateDriftingParticle);
}

INCLUDE_ASM("asm/nonmatchings/6DD0", updateDriftingParticle);

void cleanupDriftingParticle(ParticleState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}

void initTrailingParticle(ParticleState *arg0) {
    func_80006940_inner *inner = arg0->owner;

    if (inner->unkC == 0x4F) {
        loadSpriteAsset(&arg0->spriteState, 6);
    } else {
        loadSpriteAsset(&arg0->spriteState, 3);
    }
    setSpriteAnimation(&arg0->spriteState, 0x10000, 0, -1);
    setCleanupCallback(cleanupTrailingParticle);
    setCallback(updateTrailingParticle);
}

void updateTrailingParticle(ParticleState *arg0) {
    s8 unused[2];
    func_80006940_inner *inner;
    func_80006940_inner *a0_inner;
    s32 posX;
    s32 posY;
    s32 posZ;

    do {
        unused[0] = 1;
    } while (0);

    do {
        unused[1] = -1;
    } while (0);

    inner = arg0->owner;

    if (inner->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    posX = inner->unk2C;
    posY = inner->unk30;
    posZ = inner->unk34;

    updateSpriteAnimation(&arg0->spriteState, 0x10000);

    a0_inner = arg0->owner;
    if (a0_inner->unk88 != 0) {
        if (a0_inner->displayEnabled != 0) {
            func_8000A13C_AD3C(&arg0->spriteState, a0_inner->unk10->unk16, posX, posY, posZ, 0x10000, 0x10000, 0, 0);
        }
    }
}

void cleanupTrailingParticle(ParticleState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->spriteState);
}
