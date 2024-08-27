SRCDIR = src
BUILDDIR = bin
SCRIPTSDIR = Scripts
BUILDTYPE ?= Debug

CXX = g++
AS = nasm
AR = ar
OBJCPY = objcopy
VALGRIND = valgrind
PYTHON = python3

CXXFLAGS = -Wall -Wextra -Werror -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ASFLAGS = -Werror -f elf64 -I $(SRCDIR) -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ARFLAGS = -rcs
OBJCPYFLAGS = -O elf64-x86-64 -B i386 -I binary
VALGRINDFLAGS = -s --leak-check=full --show-leak-kinds=all

ifeq ($(BUILDTYPE), Debug)
ASFLAGS += -g -O0 -DDebug
CXXFLAGS += -g -O0 -DDebug
else
CXXFLAGS += -O2
endif

HEADERS = $(shell find $(SRCDIR) -type f -name "*.hpp")
HEADERS += $(shell find $(SRCDIR)/Platform -type f -name "*.cpp")
HEADERS += $(SRCDIR)/Lib/MathLib.hpp

SRCXX = $(shell find $(SRCDIR)/Lib -type f -name "*.cpp")
SRCPSF = $(shell find $(SRCDIR)/Lib -type f -name "*.psf")
OBJS = $(patsubst $(SRCDIR)/Lib/%.psf, $(BUILDDIR)/Objects/Lib/%.o, $(SRCPSF))

$(BUILDDIR)/libExtras.a: $(OBJS)
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $^
	@echo "==> Created: $@"
$(BUILDDIR)/Objects/%.o: $(SRCDIR)/%.psf Makefile $(BUILDDIR)/Targets.mk
	@mkdir -p $(@D)
	@$(OBJCPY) $(OBJCPYFLAGS) $< $@
	@echo "==> Created: $@"
$(BUILDDIR)/Targets.mk: $(SCRIPTSDIR)/Config.py Targets.txt
	@mkdir -p $(@D)
	@$(PYTHON) $^ $@
	@echo "==> Created: $@"
$(SRCDIR)/Lib/MathLib.hpp: $(SCRIPTSDIR)/MakeIncludes.py $(SRCXX)
	@mkdir -p $(@D)
	@$(PYTHON) $< $(patsubst $(SRCDIR)/Lib/%.cpp, %.cpp, $(SRCXX)) $@
	@echo "==> Created: $@"

MATHPROGRAMS ?= $(shell find $(SRCDIR)/TestPrograms/Math -type f -name "*.txt")
CRYPTMSG ?= $(SRCDIR)/TestPrograms/Cryptography/CaesarCipher.txt
MLITERS ?= 20000
SERVERPORT ?= 8080
INTERPRETERPROGRAM ?= $(SRCDIR)/TestPrograms/Interpreter/Main.txt
DISKSIZE ?= 2K
DISKOUTPUT ?= $(BUILDDIR)/Disk.img
DISKTYPE ?= MBR
DISKARGS = -diskSize $(DISKSIZE) -diskType $(DISKTYPE) -output $(DISKOUTPUT)
OSROOT ?= $(SRCDIR)/TestPrograms/OS
OSCXX = x86_64-elf-$(CXX)
OSCXXFLAGS = $(CXXFLAGS) -DFreestanding -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -fno-omit-frame-pointer -fstack-protector-all
OSQEMUCMD = qemu-system-x86_64 -usb -smp 1 -M q35 -m 2048 -cdrom $(BUILDDIR)/OS.img

include $(BUILDDIR)/Targets.mk

test: $(BUILDDIR)/Tests.out
	@./$<
clean:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@touch $(SRCDIR)/Lib/MathLib.hpp
	@rm $(SRCDIR)/Lib/MathLib.hpp
	@echo "==> Deleted compiled files"
.PHONY: test clean