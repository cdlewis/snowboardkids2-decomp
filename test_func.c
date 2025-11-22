#include "3A1F0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "overlay.h"
#include "task_scheduler.h"

void func_8001BCDC_1C8DC(void) {
    GameState *state;
    s32 i, j;
    s32 count;
    s32 numPlayers;
    s32 limit;
    
    state = (GameState *)getCurrentAllocation();
    i = 0;
    
    do {
        numPlayers = D_800AFE8C_A71FC->unk8;
        count = 0;
        j = 0;
        
        if (numPlayers > 0) {
            limit = numPlayers;
            do {
                u8 value = state->unk59A[j];
                if (((value ^ 1) < 1) | ((value ^ 3) < 1)) {
                    count++;
                }
                j++;
            } while (j < limit);
        }
        
        if ((u8)count == 0) {
            state->unk5B8[i] = 0;
        }
        i++;
    } while (i < 9);
}
