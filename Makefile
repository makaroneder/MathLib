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

MKDIR = $(PYTHON) $(SCRIPTSDIR)/MakeDirectory.py
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CXXFLAGS = -Wall -Wextra -Werror -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ASFLAGS = -Werror -f elf64 -I $(SRCDIR) -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ARFLAGS = -rcs
OBJCPYFLAGS = -O elf64-x86-64 -B i386 -I binary
VALGRINDFLAGS = -s --leak-check=full --show-leak-kinds=all
ASLFLAGS = -oa

ifeq ($(BUILDTYPE), Debug)
ASFLAGS += -g -O0 -DDebug
CXXFLAGS += -g -O0 -DDebug
else
CXXFLAGS += -O2
endif

HEADERS = $(call rwildcard,$(SRCDIR),*.hpp)
HEADERS += $(call rwildcard,$(SRCDIR)/Platform,*.cpp)
HEADERS += $(SRCDIR)/Lib/MathLib.hpp

BUILDSYSDEPS = $(call rwildcard,$(SRCDIR)/BuildSystem,*.cpp)
SRCXX = $(call rwildcard,$(SRCDIR)/Lib,*.cpp)
SRCPSF = $(call rwildcard,$(SRCDIR)/Lib,*.psf)
OBJS = $(patsubst $(SRCDIR)/Lib/%.psf, $(BUILDDIR)/Lib/%.o, $(SRCPSF))

$(BUILDDIR)/libExtras.a: $(OBJS)
	@$(MKDIR) $(@D)
	@$(AR) $(ARFLAGS) $@ $^
	@echo "==> Created: $@"
$(BUILDDIR)/libMath.a: $(BUILDDIR)/LibStub.o
	@$(MKDIR) $(@D)
	@$(AR) $(ARFLAGS) $@ $<
	@echo "==> Created: $@"
$(BUILDDIR)/LibStub.o: $(SRCDIR)/Lib/MathLib.hpp $(HEADERS)
	@$(MKDIR) $(@D)
	@$(CXX) $(CXXFLAGS) -x c++ -c $< -o $@
	@echo "==> Created: $@"
$(BUILDDIR)/Lib/%.o: $(SRCDIR)/Lib/%.psf Makefile
	@$(MKDIR) $(@D)
	@$(OBJCPY) $(OBJCPYFLAGS) $< $@
	@echo "==> Created: $@"
$(SRCDIR)/Lib/MathLib.hpp: $(SCRIPTSDIR)/MakeIncludes.py $(SRCXX) Makefile
	@$(MKDIR) $(@D)
	@$(PYTHON) $< $(patsubst $(SRCDIR)/Lib/%.cpp, %.cpp, $(SRCXX)) $@
	@echo "==> Created: $@"
$(BUILDDIR)/TmpBuild.out: $(BUILDSYSDEPS) $(HEADERS) $(BUILDDIR)/libExtras.a $(BUILDDIR)/libMath.a
	@$(MKDIR) $(@D)
	@$(CXX) $(CXXFLAGS) $(BUILDSYSDEPS) -o $@ -L $(BUILDDIR) -l Extras -l Math
	@echo "==> Created: $@"
$(BUILDDIR)/Build.mk: $(BUILDDIR)/TmpBuild.out Build.txt
	@$(MKDIR) $(@D)
	@./$^ $@
	@echo "==> Created: $@"

MATHPROGRAMS ?= $(call rwildcard,$(SRCDIR)/TestPrograms/Math,*.txt)
CRYPTMSG ?= $(SRCDIR)/TestPrograms/Cryptography/CaesarCipher.txt
MLITERS ?= 20000
SERVERPORT ?= 8080
INTERPRETERPROGRAM ?= $(SRCDIR)/TestPrograms/Interpreter/Main.txt
CHEMPROGRAM ?= $(SRCDIR)/TestPrograms/Chemistry/All.txt
DISKSIZE ?= 4480
DISKOUTPUT ?= $(BUILDDIR)/Disk.img
DISKTYPE ?= MBR
CARBONS ?= 6
ANIMATIONFILE ?= $(SRCDIR)/TestPrograms/Math/Trigonometry/Sine.txt
POLYNOMIAL ?= $(SRCDIR)/TestPrograms/Math/Polynomial.txt
IMGCONVINPUTTYPE ?= TGA
IMGCONVINPUT ?= $(SRCDIR)/TestPrograms/RPG/Emoji.tga
IMGCONVOUTPUTTYPE ?= P6
IMGCONVOUTPUT ?= $(BUILDDIR)/ImageConverter.ppm
ELFPROGRAM ?= $(BUILDDIR)/ELFParser.out
4DPROGRAM ?= $(SRCDIR)/TestPrograms/4D/Tesseract.txt
DBOUTPUT ?= $(BUILDDIR)/DB.bin
IMGPROCINPUT ?= $(SRCDIR)/TestPrograms/Cards/Prototype.tga
HEADERTREEINPUT ?= $(SRCDIR)/Lib/Host.hpp
IMGGENMODEL ?= $(BUILDDIR)/ImageGenerator.bin
IMGGENINPUT ?= $(SRCDIR)/TestPrograms/ImageGenerator/AI.aseprite
FSCONVOUTPUT ?= $(BUILDDIR)/FS.tar
FSCONVINPUT ?= Makefile Makefile TODO.md TODO.md
BENCODEINPUT ?= $(SRCDIR)/TestPrograms/Bencode/ArchLinux.torrent
SERVERTYPE ?= client
SERVERTARGET ?= $(SRCDIR)/TestPrograms/Network/Target.json
COMPILERINPUT ?= $(SRCDIR)/TestPrograms/Compiler/Main.txt
COMPILEROUTPUT ?= $(BUILDDIR)/Compiler.asm
OSROOT ?= $(SRCDIR)/TestPrograms/OS
OSCXX = x86_64-elf-$(CXX)
OSCXXFLAGS = $(CXXFLAGS) -DFreestanding -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -fno-omit-frame-pointer -fstack-protector-all
OSLINKER = $(SRCDIR)/OS/Linker.ld
OSLDFLAGS = $(OSCXXFLAGS) -T $(OSLINKER) -Bsymbolic -nostdlib -Xlinker -Map=$(BUILDDIR)/Kernel.map
OSQEMUCMD = qemu-system-x86_64 -usb -smp 1 -M q35 -m 4096 -rtc base=localtime -cdrom $(BUILDDIR)/OS.img -drive file=$(BUILDDIR)/FAT.img,format=raw,media=disk -boot d

clean:
	@$(MKDIR) $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@touch $(SRCDIR)/Lib/MathLib.hpp
	@rm $(SRCDIR)/Lib/MathLib.hpp
	@echo "==> Deleted compiled files"
.PHONY: clean

include $(BUILDDIR)/Build.mk