PROCS = $(shell nproc)

PREFIX ?= /usr/local/x86_64makarongcc
TARGET ?= x86_64-makaron
TARGETS = $(TARGET),x86_64-elf,x86_64-pe
SYSROOT = $(abspath ../bin/SystemRoot)

BINUTILSVERSION ?= 2.41
GCCVERSION ?= 13.2.0

gcc: gcc-$(GCCVERSION).tar.gz binutils
	@tar xf $<
	@patch -p0 -i Patches/gcc.diff
	@cp Patches/makaron.h gcc-$(GCCVERSION)/gcc/config/
	@cd gcc-$(GCCVERSION)/libstdc++-v3 && autoconf
	@mkdir -p GCC
	@cd GCC && ../gcc-$(GCCVERSION)/configure --target=$(TARGET) --enable-targets=$(TARGETS) --prefix="$(PREFIX)" --with-sysroot=$(SYSROOT) --enable-shared --disable-nls --enable-languages=c,c++
	@$(MAKE) -s -j$(PROCS) -C GCC all-gcc
	@$(MAKE) -s -j$(PROCS) -C GCC all-target-libgcc
	@sudo $(MAKE) -s -j$(PROCS) -C GCC install-gcc
	@sudo $(MAKE) -s -j$(PROCS) -C GCC install-target-libgcc
	@echo "==> Created: $@"
binutils: binutils-$(BINUTILSVERSION).tar.gz
	@tar xf $<
	@patch -p0 -i Patches/binutils.diff
	@cp Patches/elf_i386_makaron.sh binutils-$(BINUTILSVERSION)/ld/emulparams/
	@cp Patches/elf_x86_64_makaron.sh binutils-$(BINUTILSVERSION)/ld/emulparams/
	@cd binutils-$(BINUTILSVERSION)/ld && automake
	@mkdir -p Binutils
	@cd Binutils && ../binutils-$(BINUTILSVERSION)/configure --target=$(TARGET) --prefix=$(PREFIX) --with-sysroot=$(SYSROOT) --enable-shared  --disable-nls --enable-interwork --enable-multilib --disable-werror
	@$(MAKE) -s -j$(PROCS) -C Binutils
	@sudo $(MAKE) -s -j$(PROCS) -C Binutils install
	@echo "==> Created: $@"
binutils-$(BINUTILSVERSION).tar.gz:
	@curl -O https://ftp.gnu.org/gnu/binutils/$@
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