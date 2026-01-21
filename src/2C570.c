#include "20F0.h"
#include "297B0.h"
#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern s16 signedAngleDifference(s16 fromAngle, s16 toAngle);

void func_8002C570_2D170(Func297D8Arg *arg0) {
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 var_a0;
    s16 var_v0;
    s16 var_v1;
    GameState *gameState;
    s32 i;
    s32 completedCount;

    gameState = getCurrentAllocation();
    completedCount = 0;

    if (gameState->unk41C > 0) {
        for (i = 0; i < gameState->unk41C; i++) {
            switch (arg0[i].unk32) {
                case 0:
                    temp_v0_2 = signedAngleDifference(arg0[i].unk30, arg0[i].unk2E);
                    if (!(temp_v0_2 & 0x8000)) {
                        arg0[i].unk36 = 0;
                    } else {
                        arg0[i].unk36 = 1;
                    }
                    var_v0 = temp_v0_2;
                    if (var_v0 < 0) {
                        var_v0 = -var_v0;
                    }
                    arg0[i].unk50 = 2;
                    arg0[i].unk37 = 1;
                    if (var_v0 >= 0xAAB) {
                        arg0[i].unk30 = (arg0[i].unk30 + 0x1000) & 0x1FFF;
                        arg0[i].unk50 = 1;
                        arg0[i].unk37 = 0;
                        if (!(signedAngleDifference(arg0[i].unk30, arg0[i].unk2E) & 0x8000)) {
                            arg0[i].unk36 = 0;
                        } else {
                            arg0[i].unk36 = 1;
                        }
                    }
                    arg0[i].unk32 = 1;
                    break;
                case 1:
                    temp_v0_3 = signedAngleDifference(arg0[i].unk30, arg0[i].unk2E);
                    var_v1 = temp_v0_3;
                    if (temp_v0_3 < 0) {
                        var_v1 = -var_v1;
                    }
                    var_a0 = 0xA0;
                    if (var_v1 < 0xA0) {
                        if (arg0[i].unk50 == 2) {
                            arg0[i].unk50 = 0;
                        }
                        var_a0 = var_v1;
                        arg0[i].unk32 = 3;
                    }
                    if (arg0[i].unk36 != 0) {
                        var_a0 = -var_a0;
                    }
                    arg0[i].unk30 = arg0[i].unk30 + var_a0;
                    if (arg0[i].unk50 == 1) {
                        if (arg0[i].unk37 != 0) {
                            arg0[i].unk50 = 0;
                        }
                    }
                    break;
                case 3:
                    if (arg0[i].unk37 != 0) {
                        arg0[i].unk50 = 0;
                    }
                case 2:
                    completedCount++;
                    break;
            }
        }
    }

    if ((completedCount & 0xFF) == gameState->unk41C) {
        gameState->unk42A = 0;
        if (gameState->unk41C > 0) {
            for (i = 0; i < gameState->unk41C; i++) {
                arg0[i].rotation = arg0[i].unk2E;
                arg0[i].unk50 = arg0[i].unk56;
                setAnimationIndex(arg0[i].model, arg0[i].unk5F);
            }
        }
    }
}
