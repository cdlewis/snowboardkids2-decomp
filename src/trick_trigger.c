#include "common.h"
#include "event_trigger.h"
#include "gamestate.h"

void func_80069CC0_6A8C0(void*);

u8 TrickLabel[] = "TRICK";

extern GameState* GameStateGet();
extern u8 D_8008D6C4_8E2C4[];
extern void TrickTriggerCheck(u8*);

void TrickTriggerInit(EventTrigger* arg0) {
    arg0->unk0 = TRICK_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &TrickLabel;
    func_80069CC0_6A8C0(&TrickTriggerCheck);
}

void TrickTriggerCheck(u8* arg0) {
    s16 temp_v0_2;
    s16 var_v1;
    s32 temp_a1;
    GameState* temp_v0;
    s32 val1, val2;

    temp_v0 = GameStateGet();
    if (temp_v0->unk3F8 > 0x800000) {
        temp_v0_2 = temp_v0->unk3F4;
        var_v1 = temp_v0_2;
        if (temp_v0_2 >= 0x1001) {
            var_v1 -= 0x2000;
        }

        temp_a1 = *arg0;
        val1 = ((s16*)D_8008D6C4_8E2C4)[temp_a1 * 2];
        if (var_v1 < val1) {
            val2 = ((s16*)D_8008D6C4_8E2C4)[temp_a1 * 2 + 1];
            if (val2 < var_v1) {
                temp_v0->unk424 = 1;
                temp_v0->unk425 = (u8)*arg0;
            }
        }
    }
}
