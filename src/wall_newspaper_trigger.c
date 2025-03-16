#include "common.h"
#include "event_trigger.h"

typedef struct {
    char padding[0x3F4];
    s16 unk3F4;
    s32 unk3F8;
    u16 unk3FC;
    char padding2[0x26];
    u8 unk424;
    s8 unk425;
} GameState;

void func_80069CC0_6A8C0(void *);
void WallNewspaperTriggerCheck(u8 *);

u8 WallNewspaperLabel[] = "WALL NEWSPAPER";

extern GameState *GameStateGet();
extern u8 D_8008D6C4_8E2C4[];

void WallNewspaperTriggerInit(EventTrigger *arg0) {
    arg0->unk0 = WALL_NEWSPAPER_EVENT_ID;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &WallNewspaperLabel;

    func_80069CC0_6A8C0(&WallNewspaperTriggerCheck);
}

void WallNewspaperTriggerCheck(u8 *arg0) {
    s16 temp_v0_2;
    s16 var_v1;
    s32 temp_a2;
    GameState *temp_v0;
    s32 val1;
    s32 val2;

    temp_v0 = GameStateGet();
    if (temp_v0->unk3F8 > 0x760000) {
        temp_v0_2 = temp_v0->unk3F4;
        var_v1 = temp_v0_2;
        if (temp_v0_2 >= 0x1001) {
            var_v1 -= 0x2000;
        }
        temp_a2 = *arg0;
        val1 = ((s16 *)D_8008D6C4_8E2C4)[temp_a2 * 2];
        if (var_v1 < val1) {
            val2 = ((s16 *)D_8008D6C4_8E2C4)[(temp_a2 * 2) + 1];
            if (var_v1 > val2) {
                u32 temp = temp_v0->unk3FC - 0xC01;
                temp_v0->unk424 = 1;
                temp_v0->unk425 = *arg0;
            }
        }
    }
}
