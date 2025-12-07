#include "20F0.h"
#include "288A0.h"
#include "5E590.h"
#include "68CF0.h"
#include "6E840.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

extern s16 D_8008F17C_8FD7C[];
extern void func_800136E0_142E0(void);
extern void func_80032708_33308(void);

typedef struct {
    u8 padding[0x77C];
    u16 unk77C;
} func_80032628_33228_allocation;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 padding[0x14];
} func_80032628_33228_item;

void func_80032628_33228(func_80032628_33228_item *arg0) {
    func_80032628_33228_item *sp18;
    s16 *sp1C;
    s32 sp20;
    func_80032628_33228_item *sp24;
    func_80032628_33228_allocation *sp28;

    sp18 = arg0;
    sp28 = (func_80032628_33228_allocation *)getCurrentAllocation();
    sp20 = 0;
    sp1C = D_8008F17C_8FD7C;
    sp24 = arg0 + 4;

    do {
        register s16 a1;
        register s16 a0;
        register s16 v1;
        register s16 v0;
        
        a1 = sp18->unk2;
        a0 = sp1C[0];
        
        if (a1 < a0) {
            v0 = a0 - a1;
            if (v0 < 0) {
                a0 = -v0;
            } else {
                a0 = v0;
            }

            if (a0 < 0x14) {
                v0 = a1 + a0;
            } else {
                v0 = a1 + 0x14;
            }
            sp18->unk2 = v0;
            sp20++;
        }

        debugEnqueueCallback(8, 0, func_800136E0_142E0, sp18);

        sp18++;
        sp1C++;
    } while (sp18 < sp24);

    if ((sp20 & 0xFF) == 0) {
        sp28->unk77C = 1;
        setCallback(func_80032708_33308);
    }
}
