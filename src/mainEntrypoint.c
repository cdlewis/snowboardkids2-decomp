#include "PRinternal/viint.h"
#include "common.h"

typedef struct
{
    u32 field_0x0;  // Likely a status or control field, initialized to 0
    u32 field_0x4;  // Pointer to a linked resource or structure
    u32 field_0x8;  // Stores the value from $a1 (related pointer or identifier)
    u16 field_0xC;  // Initialized to 0, could be a flag or small data
    u16 field_0xE;  // Derived from $a2 - 1, likely a size or count
} D_800A32D0_Type;

extern void func_800395F0_3A1F0();
extern void func_80039B88_3A788();
extern void func_80039C34_3A834();
extern void func_8003AC38_3B838();
extern void func_8003AF6C_3BB6C();
extern void func_8003B5A0_3C1A0();
extern void func_80057870_58470(s32);
extern void func_800692F0_69EF0();
extern void func_800693C4_69FC4(void *, s32);
extern void func_80069530_6A130();
extern void func_8006A220_6AE20();
extern void func_8006A428_6B028();
extern void func_8006DC40_6E840();
extern void func_8006E000_6EC00(u16);
extern void func_8006E02C_6EC2C();
extern void func_8006E054_6EC54(s32);
extern void func_800701B0_70DB0(s32);
extern void func_800705D0_711D0(D_800A32D0_Type *, OSMesgQueue *, s32);
extern u16 D_8009ADE0_9B9E0;
extern void D_800A32D0_A3ED0;
extern s8 D_800AFCE2_A7052;
extern void func_80056080_56C80;
extern void func_80067FB0_68BB0(void *, s32);

void mainThreadEntrypoint(void *arg) {
    OSMesg D_800A32E0_A3EE0;
    OSMesgQueue D_800AB1B0_A2520;

    int new_var;
    s32 message;
    s32 var_s0;
    s32 var_s1 = 0;
    s32 var_s2;

    if (osTvType == OS_TV_TYPE_NTSC) {
        func_800701B0_70DB0(2);
    } else {
        func_800701B0_70DB0(0x1E);
    }

    func_800395F0_3A1F0();
    func_8006A220_6AE20();
    func_8006DC40_6E840();
    func_8003B5A0_3C1A0();
    func_800692F0_69EF0();

    osCreateMesgQueue(&D_800AB1B0_A2520, &D_800A32E0_A3EE0, 0x20);

    func_800705D0_711D0(&D_800A32D0_A3ED0, &D_800AB1B0_A2520, 2);
    new_var = 0x64;
    func_800693C4_69FC4(&func_80067FB0_68BB0, new_var);
    func_800693C4_69FC4(&func_80056080_56C80, 0xFA);

    while (TRUE) {
        osRecvMesg(&D_800AB1B0_A2520, (OSMesg *)&message, 1);
        var_s0 = 0;

        do {
            if (*(u16 *)&message != 0x5) {
                if (message < 6) {
                    if (message < 2 && message >= 0) {
                        func_8006E000_6EC00(message);

                        if (var_s0 == 0) {
                            func_8006E02C_6EC2C();
                        }
                    }
                } else if (message == 0xA) {
                    var_s1 = 1;
                    func_80057870_58470(0x14);
                }
            } else {
                var_s2 = message;
                var_s0 = 1;
            }
        } while (osRecvMesg(&D_800AB1B0_A2520, (OSMesg *)&message, 0) != -1);

        if (var_s0 == 0) {
            continue;
        }

        if (var_s1 == 0) {
            func_80039C34_3A834();
            func_80039B88_3A788();
            D_8009ADE0_9B9E0++;
            func_8006A428_6B028();
            func_80069530_6A130();
            func_8003AC38_3B838();
            func_8006E054_6EC54((u16)var_s2);
            continue;
        }

        D_800AFCE2_A7052 = 1;
        func_8003AF6C_3BB6C();
        func_8003AC38_3B838();
    }
}
