#include "70DB0.h"
#include "common.h"

extern s32 D_800A6464_A7064;
extern s32 D_800A6468_A7068;
extern s32 D_800A6498_A7098;
extern s32 D_8009B044_9BC44;
extern u8 diskrom_handle;
extern void func_800712D4_71ED4();
extern OSMesgQueue D_800A8F58_A02C8;
extern u32 D_8009B030_9BC30;
extern void *D_8009B040_9BC40;
extern ALGlobals __libmus_alglobals;
extern void addViConfig(ViConfig *, OSMesgQueue *, s32);
extern s32 func_800710EC_71CEC(void *, void *);
extern void func_80071294_71E94(void *);

typedef struct {
    u16 type;
    u16 pad;
    void *data;
} Msg;

INCLUDE_ASM("asm/nonmatchings/717D0", __MusIntAudManInit);

void func_80071000_71C00(void) {
    ViConfig cfg;
    OSMesg msg;
    OSMesg msg2;
    OSMesgQueue *q = &D_800A8F58_A02C8;
    s32 quit = 0;
    addViConfig(&cfg, q, 1);
    while (quit == 0) {
        osRecvMesg(q, &msg, 1);
        osRecvMesg(q, 0, 0 * 0);
        if (((Msg *)(&msg))->type == 5) {
            void **tbl = (void **)(((u8 *)q) - 0x1C0);
            void *ptr = tbl[D_8009B030_9BC30 & 3];
            if (func_800710EC_71CEC(ptr, D_8009B040_9BC40)) {
                osRecvMesg((OSMesgQueue *)(((u8 *)q) + 0x38), &msg2, 1);
                ;
                func_80071294_71E94(*((void **)(((u8 *)msg2) + 4)));
                D_8009B040_9BC40 = *((void **)(((u8 *)msg2) + 4));
            }
        }
    }

    alClose(&__libmus_alglobals);
}

INCLUDE_ASM("asm/nonmatchings/717D0", func_800710EC_71CEC);

void func_80071294_71E94(void *arg0) {
    if (((osAiGetLength() >> 2) == 0) && (D_8009B044_9BC44 == 0)) {
        D_8009B044_9BC44 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/717D0", func_800712D4_71ED4);

void *func_800714D8_720D8(void *arg0) {
    u8 *handle = (u8 *)&diskrom_handle;

    if (*handle == 0) {
        u32 value = D_800A6498_A7098;
        D_800A6464_A7064 = 0;
        *handle = 1;
        D_800A6468_A7068 = value;
    }

    *(void **)arg0 = handle;
    return &func_800712D4_71ED4;
}

INCLUDE_ASM("asm/nonmatchings/717D0", func_80071518_72118);
