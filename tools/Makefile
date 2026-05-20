KMC_DIR         := gcc_kmc
KMC_GCC         := $(KMC_DIR)/gcc
KMC_BINUTILS    := $(KMC_DIR)/as

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  PLATFORM := linux
else ifeq ($(UNAME_S),Darwin)
  PLATFORM := mac
else
  $(error Unsupported platform for KMC toolchain.)
endif

all: $(KMC_GCC) $(KMC_BINUTILS)

clean:
	$(RM) -rf $(KMC_DIR)

distclean: clean

.PHONY: all

$(KMC_DIR):
	mkdir -p $@

$(KMC_GCC): | $(KMC_DIR)
	@mkdir -p $(KMC_DIR)
	curl -O -L https://github.com/decompals/mips-gcc-2.7.2/releases/latest/download/gcc-2.7.2-$(PLATFORM).tar.gz
	tar xf gcc-2.7.2-$(PLATFORM).tar.gz -C $(KMC_DIR)
	rm gcc-2.7.2-$(PLATFORM).tar.gz

$(KMC_BINUTILS): | $(KMC_DIR)
	@mkdir -p $(KMC_DIR)
	curl -O -L https://github.com/decompals/mips-binutils-2.6/releases/latest/download/binutils-2.6-$(PLATFORM).tar.gz
	tar xf binutils-2.6-$(PLATFORM).tar.gz -C $(KMC_DIR)
	rm binutils-2.6-$(PLATFORM).tar.gz
