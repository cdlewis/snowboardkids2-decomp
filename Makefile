MAKEFLAGS += -j$(shell nproc)

BASENAME  = snowboardkids2

# Colours

NO_COL  := \033[0m
RED     := \033[0;31m
RED2    := \033[1;31m
GREEN   := \033[0;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
PINK    := \033[0;35m
CYAN    := \033[0;36m

# Directories

BUILD_DIR = build
ASM_DIRS  = asm asm/data
BIN_DIRS  = assets
TOOLS_DIR = tools
SRC_DIRS  = src

# Files

S_FILES   = $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
C_FILES   = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
BIN_FILES = $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))
LIBKMC_S_FILES = $(foreach dir,lib/libkmc,$(wildcard $(dir)/*.s))
O_FILES := $(shell grep -E 'build\/(asm|assets|src|src\/entrypoint|bin|lib\/libkmc)\/.+\.o' snowboardkids2.ld -o | sort | uniq)

# Tools

CROSS = mips-linux-gnu-
AS = $(CROSS)as
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON = python3
SPLAT = $(TOOLS_DIR)/splat/split.py
N64CRC = $(TOOLS_DIR)/n64crc.py
CPP := $(CROSS)cpp
ICONV := iconv --from-code=UTF-8 --to-code=Shift-JIS

COMPILER_DIR = tools/gcc_kmc
CC := COMPILER_PATH=$(COMPILER_DIR) $(COMPILER_DIR)/gcc
CC_CHECK := clang

# Flags

MACROS := -D_LANGUAGE_C -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_MIPS_SZLONG=32 -D__USE_ISOC99 -DF3DEX_GBI_2 -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
ABIFLAG ?= -mabi=32 -mgp32 -mfp32
CFLAGS := $(ABIFLAG) -mno-abicalls -nostdinc -fno-PIC -G 0 -Wa,-force-n64align -funsigned-char -w -mips3 -EB -O2 -fno-builtin
IINC := -I include -I lib/ultralib/include -I lib/ultralib/include/PR -I lib/libmus/include/PR -I lib/libmus/src -I lib/f3dex2/PR

MIPS_BUILTIN_DEFS := -D_MIPS_ISA_MIPS2=2 -D_MIPS_ISA=_MIPS_ISA_MIPS2 -D_ABIO32=1 -D_MIPS_SIM=_ABIO32 -D_MIPS_SZINT=32 -D_MIPS_SZPTR=32
CC_CHECK_FLAGS    := -MMD -MP -fno-builtin -fsyntax-only -fdiagnostics-color -std=gnu89 -m32 -DNON_MATCHING -DAVOID_UB -DCC_CHECK=1 -Werror=implicit-function-declaration -Werror=strict-prototypes

TARGET = $(BUILD_DIR)/$(BASENAME)

OBJCOPYFLAGS = -O binary

ASFLAGS = -G 0 -I include -mips3 -mabi=32 $(GRUCODE_ASFLAGS)

LIBULTRA = lib/ultralib/build/J/libgultra_rom/libgultra_rom.a
LIBMUS = lib/libmus/build/libmus.a

# note: his is probably an issue with headers. Once ultra headers are properly included this should
# go away.
UNDEFINED_SYMS := osPfsIsPlug
LD_SCRIPT = $(BASENAME).ld
LINKER_SCRIPTS := linker_scripts/hardware_regs.ld linker_scripts/libultra_syms.ld
LD_FLAGS := -T $(LD_SCRIPT) -T undefined_syms_auto.txt -Map snowboardkids2.map --no-check-sections $(foreach sym,$(UNDEFINED_SYMS),-u $(sym)) -Lbuild/lib -lmus -lgultra_rom

ifeq ($(NON_MATCHING),1)
MACROS += -DNON_MATCHING
endif

# Targets

default: all

all: dirs verify

nonmatching: dirs no_verify

dirs:
	$(foreach dir,$(ASM_DIRS) $(BIN_DIRS),$(shell mkdir -p $(BUILD_DIR)/$(dir)))

verify: $(TARGET).z64
	@shasum --check $(BASENAME).sha1

no_verify: $(TARGET).z64
	@echo "Skipping SHA1SUM check, updating CRC"
	@$(PYTHON) $(N64CRC) $(TARGET).z64

extract:
	$(PYTHON) $(SPLAT) $(BASENAME).yaml

clean:
	rm -rf asm
	rm -rf assets
	rm -rf build
	rm -f *auto.txt

clean-ultralib:
	rm -r lib/ultralib/build

clean-libmus:
	rm -r lib/libmus/build

diff-line:
	@(diff --old-line-format='OLD: %L' --new-line-format='NEW: %L' <(xxd snowboardkids2.z64) <(xxd build/snowboardkids2.z64) || true) > romdiff

diff-sxs:
	@(diff -y <(xxd snowboardkids2.z64) <(xxd build/snowboardkids2.z64) || true) > romdiff

format:
	clang-format -i -style=file $(C_FILES)

$(TARGET).elf: $(BASENAME).ld $(BUILD_DIR)/lib/libgultra_rom.a $(BUILD_DIR)/lib/libmus.a $(O_FILES)
	@printf "[$(PINK) linker $(NO_COL)]  Linking $(TARGET).elf\n"
	@$(LD) $(LD_FLAGS) $(foreach ld, $(LINKER_SCRIPTS), -T $(ld)) -o $@

$(BUILD_DIR)/src/%.o: src/%.c
	@mkdir -p $(shell dirname $@)
	@printf "[$(GREEN)   c    $(NO_COL)]  $<\n"; \
	$(CC_CHECK) $(CC_CHECK_FLAGS) $(IINC) $(MACROS) -I $(dir $*) -I src/ -I $(BUILD_DIR)/$(dir $*) -o $@ $<
	@$(CC) $(CFLAGS) -fno-asm $(IINC) $(MACROS) -I $(dir $*) -I src/ -I $(BUILD_DIR)/$(dir $*) -E $< | $(CC) -x c $(CFLAGS) -fno-asm -I $(dir $*) -c -o $@ -
	$(OBJDUMP_CMD)

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(shell dirname $@)
	@printf "[$(GREEN)   as   $(NO_COL)]  $<\n"; \
	cpp -P -I include -I $(BUILD_DIR)/$(dir $*) $< | \
		$(ICONV) | \
		$(AS) $(ASFLAGS) -I $(dir $*) -I $(BUILD_DIR)/$(dir $*) -o $@
	$(OBJDUMP_CMD)

$(BUILD_DIR)/lib/libgultra_rom.a: $(LIBULTRA)
	@mkdir -p $$(dirname $@)
	@cp $< $@

$(BUILD_DIR)/lib/libmus.a: $(LIBMUS)
	@mkdir -p $$(dirname $@)
	@cp $< $@

LIBULTRA_FLAGS = VERSION=J TARGET=libgultra_rom COMPARE=0 MODERN_LD=1 GBIDEFINE="DF3DEX_GBI_2=1"
$(LIBULTRA):
	$(LIBULTRA_FLAGS) $(MAKE) -C lib/ultralib setup
	$(LIBULTRA_FLAGS) $(MAKE) -C lib/ultralib

LIBMUS_FLAGS = COMPILER_DIR=../../$(COMPILER_DIR) ULTRA_DIR=../ultralib
$(LIBMUS):
	$(LIBMUS_FLAGS) $(MAKE) -C lib/libmus

$(BUILD_DIR)/%.o: %.bin
	@printf "[$(PINK) linker $(NO_COL)]  $<\n"
	@$(LD) -r -b binary -o $(BUILD_DIR)/$(basename $<).o $<

$(TARGET).bin: $(TARGET).elf
	@printf "[$(CYAN) objcpy $(NO_COL)]  $<\n"
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(TARGET).z64: $(TARGET).bin
	@cp $< $@

setup:
	$(MAKE) -C tools

### Settings
.SECONDARY:
.PHONY: all clean default updatediff
SHELL = /bin/bash -e -o pipefail

# Print target for debugging
print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
