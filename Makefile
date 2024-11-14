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

# Files

S_FILES   = $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
BIN_FILES = $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))

O_FILES := $(foreach file,$(S_FILES),$(BUILD_DIR)/$(dir $(file))$(notdir $(basename $(file))).o) \
           $(foreach file,$(BIN_FILES),$(BUILD_DIR)/$(basename $(file)).o)

# Tools

CROSS = mips-linux-gnu-
AS = $(CROSS)as
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON = python3
SPLAT = $(TOOLS_DIR)/splat/split.py
N64CRC = $(TOOLS_DIR)/n64crc.py

# Flags

TARGET = $(BUILD_DIR)/$(BASENAME)
LD_SCRIPT = $(BASENAME).ld

OBJCOPYFLAGS = -O binary

ASFLAGS = -EB -mtune=vr4300 -march=vr4300 -mabi=32 -I include

LDFLAGS = -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -T $(LD_SCRIPT) -Map $(TARGET).map --no-check-sections

# Targets

default: all

all: dirs no_verify

dirs:
	$(foreach dir,$(ASM_DIRS) $(BIN_DIRS),$(shell mkdir -p $(BUILD_DIR)/$(dir)))

no_verify: $(TARGET).z64
	@echo "Skipping SHA1SUM check, updating CRC"
	#@$(PYTHON) $(N64CRC) $(TARGET).z64


extract:
	$(PYTHON) $(SPLAT) $(BASENAME).yaml

clean:
	rm -rf asm
	rm -rf assets
	rm -rf build
	rm -f *auto.txt

$(TARGET).elf: $(O_FILES)
	@$(LD) $(LDFLAGS) -o $@
	@printf "[$(PINK) linker $(NO_COL)]  $<\n"

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $(BUILD_DIR)/$(dir $<)$(notdir $(basename $<)).o
	@printf "[$(GREEN)   as   $(NO_COL)]  $<\n"

$(BUILD_DIR)/%.o: %.bin
	./patch_linker.sh
	$(LD) -r -b binary -o $(BUILD_DIR)/$(basename $<).o $<
	@printf "[$(PINK) linker $(NO_COL)]  $<\n"


$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
	@printf "[$(CYAN) objcpy $(NO_COL)]  $<\n"

$(TARGET).z64: $(TARGET).bin
	@cp $< $@

### Settings
.SECONDARY:
.PHONY: all clean default
SHELL = /bin/bash -e -o pipefail

