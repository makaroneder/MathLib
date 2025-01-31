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

BUILDSYSDEPS = $(shell find $(SRCDIR)/BuildSystem -type f -name "*.cpp")
SRCXX = $(shell find $(SRCDIR)/Lib -type f -name "*.cpp")
SRCPSF = $(shell find $(SRCDIR)/Lib -type f -name "*.psf")
OBJS = $(patsubst $(SRCDIR)/Lib/%.psf, $(BUILDDIR)/Lib/%.o, $(SRCPSF))

$(BUILDDIR)/libExtras.a: $(OBJS)
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $^
	@echo "==> Created: $@"
$(BUILDDIR)/libMath.a: $(BUILDDIR)/Lib/LibStub.o
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $<
	@echo "==> Created: $@"
$(BUILDDIR)/Lib/LibStub.o: $(SRCDIR)/LibStub/Main.cpp $(SRCDIR)/Lib/MathLib.hpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "==> Created: $@"
$(BUILDDIR)/Lib/%.o: $(SRCDIR)/Lib/%.psf Makefile
	@mkdir -p $(@D)
	@$(OBJCPY) $(OBJCPYFLAGS) $< $@
	@echo "==> Created: $@"
$(SRCDIR)/Lib/MathLib.hpp: $(SCRIPTSDIR)/MakeIncludes.py $(SRCXX) Makefile
	@mkdir -p $(@D)
	@$(PYTHON) $< $(patsubst $(SRCDIR)/Lib/%.cpp, %.cpp, $(SRCXX)) $@
	@echo "==> Created: $@"
$(BUILDDIR)/TmpBuild.out: $(SRCDIR)/BuildSystem/Main.cpp $(BUILDSYSDEPS) $(HEADERS) $(BUILDDIR)/libExtras.a $(BUILDDIR)/libMath.a
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -DIncludeSources $< -o $@ -L $(BUILDDIR) -l Extras -l Math
	@echo "==> Created: $@"
$(BUILDDIR)/Build.mk: $(BUILDDIR)/TmpBuild.out Build.txt
	@mkdir -p $(@D)
	@./$^ $@
	@echo "==> Created: $@"

MATHPROGRAMS ?= $(shell find $(SRCDIR)/TestPrograms/Math -type f -name "*.txt")
CRYPTMSG ?= $(SRCDIR)/TestPrograms/Cryptography/CaesarCipher.txt
MLITERS ?= 20000
SERVERPORT ?= 8080
INTERPRETERPROGRAM ?= $(SRCDIR)/TestPrograms/Interpreter/Main.txt
CHEMPROGRAM ?= $(SRCDIR)/TestPrograms/Chemistry/All.txt
DISKSIZE ?= 4480
DISKOUTPUT ?= $(BUILDDIR)/Disk.img
DISKTYPE ?= MBR
DISKARGS = -diskSize $(DISKSIZE) -diskType $(DISKTYPE) -output $(DISKOUTPUT)
CARBONS ?= 6
ANIMATIONFILE ?= $(SRCDIR)/TestPrograms/Math/Trigonometry/Sine.txt
POLYNOMIAL ?= $(SRCDIR)/TestPrograms/Math/Polynomial.txt
IMGCONVINPUTTYPE ?= TGA
IMGCONVINPUT ?= $(SRCDIR)/TestPrograms/RPG/Emoji.tga
IMGCONVOUTPUTTYPE ?= P6
IMGCONVOUTPUT ?= $(BUILDDIR)/ImageConverter.ppm
IMGCONVARGS = -inputType $(IMGCONVINPUTTYPE) -input $(IMGCONVINPUT) -outputType $(IMGCONVOUTPUTTYPE) -output $(IMGCONVOUTPUT)
ELFPROGRAM ?= $(BUILDDIR)/ELFParser.out
ELFARGS = -program $(ELFPROGRAM)
4DPROGRAM ?= $(SRCDIR)/TestPrograms/4D/Tesseract.txt
4DARGS ?= -program $(4DPROGRAM)
GAME ?= $(SRCDIR)/TestPrograms/Game/Game.json
GAMEARGS = -program $(GAME)
DBOUTPUT ?= $(BUILDDIR)/DB.bin
DBARGS = -program $(DBOUTPUT)
IMGPROCINPUT ?= $(SRCDIR)/TestPrograms/Cards/Prototype.tga
IMGPROCARGS = -input $(IMGPROCINPUT)
OSROOT ?= $(SRCDIR)/TestPrograms/OS
OSCXX = x86_64-elf-$(CXX)
OSCXXFLAGS = $(CXXFLAGS) -DFreestanding -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -fno-omit-frame-pointer -fstack-protector-all
OSLINKER = $(SRCDIR)/OS/Linker.ld
OSLDFLAGS = $(OSCXXFLAGS) -T $(OSLINKER) -Bsymbolic -nostdlib -Xlinker -Map=$(BUILDDIR)/Kernel.map
OSQEMUCMD = qemu-system-x86_64 -usb -smp 1 -M q35 -m 4096 -rtc base=localtime -cdrom $(BUILDDIR)/OS.img -drive file=$(BUILDDIR)/FAT.img,format=raw,media=disk -boot d

clean:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@touch $(SRCDIR)/Lib/MathLib.hpp
	@rm $(SRCDIR)/Lib/MathLib.hpp
	@echo "==> Deleted compiled files"
.PHONY: clean

include $(BUILDDIR)/Build.mk