BASENAME  = snowboardkids2

BUILD_DIR = build
ASM_DIRS  = asm asm/data
BIN_DIRS  = assets

BLASTCORP_EXTRACTED := assets/9FDF0.bin assets/boot.bin

TOOLS_DIR := tools

S_FILES   = $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
BIN_FILES = $(foreach dir,$(BIN_DIRS),$(wildcard $(dir)/*.bin))

O_FILES := $(foreach file,$(S_FILES),$(BUILD_DIR)/$(dir $(file))$(notdir $(basename $(file))).o) \
           $(foreach file,$(BIN_FILES),$(BUILD_DIR)/$(basename $(file)).o)


TARGET = $(BUILD_DIR)/$(BASENAME)
LD_SCRIPT = $(BASENAME).ld

CROSS = mips-linux-gnu-
AS = $(CROSS)as
CPP = cpp
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
PYTHON = python3
GZIP = gzip

OBJCOPYFLAGS = -O binary

ASFLAGS = -EB -mtune=vr4300 -march=vr4300 -mabi=32 -I include

LDFLAGS = -T $(LD_SCRIPT) -Map $(TARGET).map --no-check-sections

### Targets

default: all

all: dirs $(TARGET).z64

dirs:
	$(foreach dir,$(ASM_DIRS) $(BIN_DIRS),$(shell mkdir -p $(BUILD_DIR)/$(dir)))

extract: assets/init.bin

clean:
	rm -rf asm
	rm -rf assets
	rm -rf build
	rm -f *auto.txt
	rm -rf $(BLASTCORP_EXTRACTED)

decompress: $(BLASTCORP_EXTRACTED)

### Recipes

# decompression

blastcorps/init.bin: assets/init.bin
	cp assets/init.bin $@

assets/init.bin:
	python3 -m splat split $(BASENAME).yaml

$(TARGET).elf: $(O_FILES)
	@$(LD) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $(BUILD_DIR)/$(dir $<)$(notdir $(basename $<)).o

$(BUILD_DIR)/%.o: %.bin
	./patch_linker.sh
	$(LD) -r -b binary -o $(BUILD_DIR)/$(basename $<).o $<


$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(TARGET).z64: $(TARGET).bin
	@cp $< $@

### Settings
.SECONDARY:
.PHONY: all clean default
SHELL = /bin/bash -e -o pipefail

