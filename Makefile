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

O_FILES := $(foreach file,$(S_FILES),$(BUILD_DIR)/$(dir $(file))$(notdir $(basename $(file))).o) \
           $(foreach file,$(C_FILES),$(BUILD_DIR)/$(dir $(file))$(notdir $(basename $(file))).o) \
           $(foreach file,$(BIN_FILES),$(BUILD_DIR)/$(dir $(file))$(notdir $(basename $(file))).o) \
					 $(foreach file,$(LIBKMC_S_FILES),$(BUILD_DIR)/$(dir $(file))$(notdir $(basename $(file))).o)

# Tools

CROSS = mips-linux-gnu-
AS = $(CROSS)as
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON = python3
SPLAT = $(TOOLS_DIR)/splat/split.py
N64CRC = $(TOOLS_DIR)/n64crc.py
CPP := cpp -P
ICONV := iconv --from-code=UTF-8 --to-code=Shift-JIS

COMPILER_DIR = tools/gcc_kmc
CC = COMPILER_DIR=$(COMPILER_DIR) $(COMPILER_DIR)/cc1 

CC_CHECK := gcc -fsyntax-only -fsigned-char -nostdinc -fno-builtin -I include -I $(BUILD_DIR)/include -I src\
	-D CC_CHECK\
	-std=gnu90 -Wall -Wextra -Wno-format-security -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast $(GRUCODE_CFLAGS)

# Flags

GRUCODE_CFLAGS := -DF3DEX_GBI_2

TARGET = $(BUILD_DIR)/$(BASENAME)
LD_SCRIPT = $(BASENAME).ld

OBJCOPYFLAGS = -O binary

ASFLAGS = -G 0 -I include -mips3 -mabi=32 $(GRUCODE_ASFLAGS)

LIBULTRA = lib/ultralib/build/J/libgultra_rom/libgultra_rom.a
LIBMUS = lib/libmus/build/libmus.a

# note: his is probably an issue with headers. Once ultra headers are properly included this should
# go away.
UNDEFINED_SYMS := osPfsIsPlug
LD_FLAGS_EXTRA += $(foreach sym,$(UNDEFINED_SYMS),-u $(sym))

# Targets

default: all

all: dirs verify

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

$(TARGET).elf: $(BASENAME).ld $(BUILD_DIR)/lib/libgultra_rom.a $(BUILD_DIR)/lib/libmus.a $(O_FILES)
	@$(LD) -T $(LD_SCRIPT) -T undefined_syms_auto.txt $(LD_FLAGS_EXTRA) -Map $(TARGET).map --no-check-sections -Lbuild/lib -lgultra_rom -lmus -o $@
	@printf "[$(PINK) linker $(NO_COL)]  Linking $(TARGET).elf\n"

$(BUILD_DIR)/src/%.i: src/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC_CHECK) -MMD -MP -MT $@ -MF $@.d $<
	$(CPP) -MMD -MP -MT $@ -MF $@.d -I include/ -o $@ $<

$(BUILD_DIR)/src/%.s: $(BUILD_DIR)/src/%.i
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/lib/libkmc/%.o: lib/libkmc/%.s
	@mkdir -p $(shell dirname $@)
	@printf "[$(GREEN)   as   $(NO_COL)]  $<\n"; \
	$(CPP) -I include -I $(dir $*) -I $(BUILD_DIR)/$(dir $*) $< | \
		$(ICONV) | \
		$(AS) $(ASFLAGS) -I $(dir $*) -I $(BUILD_DIR)/$(dir $*) -o $@
	$(OBJDUMP_CMD)

$(BUILD_DIR)/lib/libgultra_rom.a: $(LIBULTRA)
	@mkdir -p $$(dirname $@)
	@cp $< $@

$(BUILD_DIR)/lib/libmus.a: $(LIBMUS)
	@mkdir -p $$(dirname $@)
	@cp $< $@

LIBULTRA_FLAGS = VERSION=J TARGET=libgultra_rom COMPARE=0 MODERN_LD=1
$(LIBULTRA):
	$(LIBULTRA_FLAGS) $(MAKE) -C lib/ultralib setup
	$(LIBULTRA_FLAGS) $(MAKE) -C lib/ultralib

LIBMUS_FLAGS = COMPILER_DIR=../../$(COMPILER_DIR) ULTRA_DIR=../ultralib
$(LIBMUS):
	$(LIBMUS_FLAGS) $(MAKE) -C lib/libmus

$(BUILD_DIR)/%.o: %.s
	@if [ "$(dir $<)" != "lib/libkmc/" ]; then \
		printf "[$(GREEN)   as   $(NO_COL)]  $<\n"; \
		$(AS) $(ASFLAGS) $< -o $(BUILD_DIR)/$(dir $<)$(notdir $(basename $<)).o; \
	fi

$(BUILD_DIR)/%.o: %.bin
	./patch_linker.sh
	$(LD) -r -b binary -o $(BUILD_DIR)/$(basename $<).o $<
	@printf "[$(PINK) linker $(NO_COL)]  $<\n"

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
	@printf "[$(CYAN) objcpy $(NO_COL)]  $<\n"

$(TARGET).z64: $(TARGET).bin
	@cp $< $@

setup:
	$(MAKE) -C tools

### Settings
.SECONDARY:
.PHONY: all clean default updatediff
SHELL = /bin/bash -e -o pipefail

