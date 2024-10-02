.set leoBootID, 0x800001A0

.set D_A4620000, 0xA4620000

.set osAppNmiBuffer, 0x8000031C;

.set D_80000000, 0x80000000
.set D_80000002, 0x80000002
.set D_80000004, 0x80000004
.set D_80000008, 0x80000008
.set D_8000000C, 0x8000000C
.set D_80000010, 0x80000010

# Assembly Macros

.set func_84001064, 0x84001064
.set func_8400103C, 0x8400103C
.set func_84001008, 0x84001008
.set K0BASE, 0x80000000
.set K1BASE, 0xA0000000
.set K2BASE, 0xC0000000

.set SI_STATUS_REG, 0xA4800018
.set SI_PIF_ADDR_RD64B_REG, 0xA4800004
.set SI_PIF_ADDR_WR64B_REG, 0xA4800010;

.set PI_STATUS_REG, 0xA4600010
.set PI_BSD_DOM1_LAT_REG, 0xA4600014;
.set PI_BSD_DOM1_PWD_REG,  0xA4600018;
.set PI_BSD_DOM1_PGS_REG,  0xA460001C;
.set PI_BSD_DOM1_RLS_REG,  0xA4600020;
.set PI_BSD_DOM2_LAT_REG,  0xA4600024;
.set PI_BSD_DOM2_LWD_REG,  0xA4600028;
.set PI_BSD_DOM2_PGS_REG,  0xA460002C;
.set PI_BSD_DOM2_RLS_REG, 0xA4600030;
.set PI_DRAM_ADDR_REG, 0xa4600000;
.set PI_CART_ADDR_REG, 0xa4600004;
.set PI_RD_LEN_REG, 0xa4600008;
.set PI_WR_LEN_REG, 0xa460000c;

.set osRomBase, 0x80000308;
.set osResetType, 0x8000030C;

.set VI_STATUS_REG, 0xA4400000; 
.set VI_DRAM_ADDR_REG, 0xA4400004; 
.set VI_WIDTH_REG, 0xA4400008; 
.set VI_INTR_REG, 0xA440000C; 
.set VI_CURRENT_REG, 0xA4400010; 
.set VI_BURST_REG, 0xA4400014;
.set VI_V_SYNC_REG, 0xA4400018; 
.set VI_H_SYNC_REG, 0xA440001C; 
.set VI_LEAP_REG, 0xA4400020; 
.set VI_H_START_REG, 0xA4400024; 
.set VI_V_START_REG, 0xA4400028; 
.set VI_V_BURST_REG, 0xA440002C; 
.set VI_X_SCALE_REG, 0xA4400030; 
.set VI_Y_SCALE_REG, 0xA4400034; 

.set D_A4800000, 0xA4800000;

.set osTvType, 0x80000300;

.set SP_MEM_ADDR_REG, 0xA4040000; 
.set SP_DRAM_ADDR_REG, 0xA4040004; 
.set SP_RD_LEN_REG, 0xA4040008; 
.set SP_WR_LEN_REG, 0xA404000C; 
.set SP_STATUS_REG, 0xA4040010;
.set SP_DMA_FULL_REG, 0xA4040014;
.set SP_DMA_BUSY_REG, 0xA4040018;
.set SP_SEMAPHORE_REG, 0xA404001C; 

.set D_A4080000, 0xA4080000

.set AI_DRAM_ADDR_REG, 0xA4500000; 
.set AI_LEN_REG, 0xA4500004; 
.set AI_CONTROL_REG, 0xA4500008; 
.set AI_STATUS_REG, 0xA450000C; 
.set AI_DACRATE_REG, 0xA4500010; 
.set AI_BITRATE_REG, 0xA4500014; 

.set MI_MODE_REG, 0xA4300000; 
.set MI_VERSION_REG, 0xA4300004; 
.set MI_INTR_REG, 0xA4300008; 
.set MI_INTR_MASK_REG, 0xA430000C; 

.macro glabel label
    .global \label
    .type \label, @function
    \label:
.endm

.macro .word32 x
    .word \x
.endm

.macro jlabel label
    .global \label
    \label:
.endm

.macro dlabel label
    .global \label
    \label:
.endm

.set $fv0,          $f0
.set $fv0f,         $f1
.set $fv1,          $f2
.set $fv1f,         $f3
.set $ft0,          $f4
.set $ft0f,         $f5
.set $ft1,          $f6
.set $ft1f,         $f7
.set $ft2,          $f8
.set $ft2f,         $f9
.set $ft3,          $f10
.set $ft3f,         $f11
.set $fa0,          $f12
.set $fa0f,         $f13
.set $fa1,          $f14
.set $fa1f,         $f15
.set $ft4,          $f16
.set $ft4f,         $f17
.set $ft5,          $f18
.set $ft5f,         $f19
.set $fs0,          $f20
.set $fs0f,         $f21
.set $fs1,          $f22
.set $fs1f,         $f23
.set $fs2,          $f24
.set $fs2f,         $f25
.set $fs3,          $f26
.set $fs3f,         $f27
.set $fs4,          $f28
.set $fs4f,         $f29
.set $fs5,          $f30
.set $fs5f,         $f31
