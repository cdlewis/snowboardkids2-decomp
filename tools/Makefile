KMC_DIR         := gcc_kmc
KMC_GCC         := $(KMC_DIR)/gcc
KMC_BINUTILS    := $(KMC_DIR)/as

all: $(KMC_GCC) $(KMC_BINUTILS)

clean:
	$(RM) -rf $(KMC_DIR) gcc_kmc

distclean: clean

.PHONY: all

$(KMC_DIR):
	mkdir -p $@

$(KMC_GCC): | $(KMC_DIR)
	@mkdir -p $(KMC_DIR)
	wget https://github.com/decompals/mips-gcc-2.7.2/releases/latest/download/gcc-2.7.2-linux.tar.gz
	tar xf gcc-2.7.2-linux.tar.gz -C $(KMC_DIR)
	rm gcc-2.7.2-linux.tar.gz

$(KMC_BINUTILS): | $(KMC_DIR)
	@mkdir -p $(KMC_DIR)
	wget https://github.com/decompals/mips-binutils-2.6/releases/latest/download/binutils-2.6-linux.tar.gz
	tar xf binutils-2.6-linux.tar.gz -C $(KMC_DIR)
	rm binutils-2.6-linux.tar.gz
