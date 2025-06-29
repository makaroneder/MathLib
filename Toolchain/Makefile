PREFIX ?= /usr/local/x86_64elfgcc
TARGET ?= x86_64-elf

BINUTILSVERSION ?= 2.41
GCCVERSION ?= 13.2.0

gcc: gcc-$(GCCVERSION).tar.gz binutils
	@tar xf $<
	@mkdir -p GCC
	@cd GCC && ../gcc-$(GCCVERSION)/configure --target=$(TARGET) --prefix="$(PREFIX)" --disable-nls --disable-libssp --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
	@$(MAKE) -s -j8 -C GCC all-gcc
	@$(MAKE) -s -j8 -C GCC all-target-libgcc
	@$(MAKE) -s -j8 -C GCC all-target-libstdc++-v3
	@sudo $(MAKE) -s -j8 -C GCC install-gcc
	@sudo $(MAKE) -s -j8 -C GCC install-target-libgcc
	@sudo $(MAKE) -s -j8 -C GCC install-target-libstdc++-v3
	@echo "==> Created: $@"
binutils: binutils-$(BINUTILSVERSION).tar.gz
	@tar xf $<
	@mkdir -p Binutils
	@cd Binutils && ../binutils-$(BINUTILSVERSION)/configure --target=$(TARGET) --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$(PREFIX) --with-sysroot
	@$(MAKE) -s -j8 -C Binutils
	@sudo $(MAKE) -s -j8 -C Binutils install
	@echo "==> Created: $@"
binutils-$(BINUTILSVERSION).tar.gz:
	@curl -O http://ftp.gnu.org/gnu/binutils/$@
	@echo "==> Created: $@"
gcc-$(GCCVERSION).tar.gz:
	@curl -O https://ftp.gnu.org/gnu/gcc/gcc-$(GCCVERSION)/$@
	@echo "==> Created: $@"
clean:
	@touch binutils-$(BINUTILSVERSION).tar.gz
	@rm binutils-$(BINUTILSVERSION).tar.gz
	@touch gcc-$(GCCVERSION).tar.gz
	@rm gcc-$(GCCVERSION).tar.gz
	@mkdir -p Binutils
	@rm -rf Binutils
	@mkdir -p GCC
	@rm -rf GCC
	@mkdir -p binutils-$(BINUTILSVERSION)
	@rm -rf binutils-$(BINUTILSVERSION)
	@mkdir -p gcc-$(GCCVERSION)
	@rm -rf gcc-$(GCCVERSION)
	@echo "==> All compiled files removed"
delete: clean
	@sudo rm -rf $(PREFIX)
	@echo "==> Toolchain removed"
.PHONY: binutils gcc clean delete