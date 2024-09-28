// Peripheral/Parallel Interface Registers
PI_DRAM_ADDR_REG = 0xa4600000; // size:0x4
PI_CART_ADDR_REG = 0xa4600004; // size:0x4
PI_RD_LEN_REG = 0xa4600008; // defined:true size:0x4
PI_WR_LEN_REG = 0xa460000c; // size:0x4
D_A4600005 = 0xA4600005; // defined:True // TODO figure out its name
D_A4600006 = 0xA4600006; // defined:True // TODO figure out its name
D_A4600007 = 0xA4600007; // defined:True // TODO figure out its name
PI_STATUS_REG = 0xA4600010; // defined:True
PI_BSD_DOM1_LAT_REG = 0xA4600014; // defined:True // PI dom1 latency
PI_BSD_DOM1_PWD_REG = 0xA4600018; // defined:True // PI dom1 pulse width
PI_BSD_DOM1_PGS_REG = 0xA460001C; // defined:True // PI dom1 page size
PI_BSD_DOM1_RLS_REG = 0xA4600020; // defined:True // PI dom1 release
PI_BSD_DOM2_LAT_REG = 0xA4600024; // defined:True // PI dom2 latency
PI_BSD_DOM2_LWD_REG = 0xA4600028; // defined:True // PI dom2 pulse width
PI_BSD_DOM2_PGS_REG = 0xA460002C; // defined:True // PI dom2 page size
PI_BSD_DOM2_RLS_REG = 0xA4600030; // defined:True // PI dom2 release