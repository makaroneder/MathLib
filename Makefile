SRCDIR = src
BUILDDIR = bin
SCRIPTSDIR = Scripts
BUILDTYPE ?= Debug

CXX = g++
AS = nasm
VALGRIND = valgrind
PYTHON = python3

MKDIR = $(PYTHON) $(SCRIPTSDIR)/MakeDirectory.py
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CXXFLAGS = -Wno-packed-bitfield-compat -Wno-unused-function -Wall -Wextra -Werror -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ASFLAGS = -Werror -f elf64 -I $(SRCDIR) -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
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
HEADERS += $(SRCDIR)/Lib/Font.hpp
HEADERS += $(SRCDIR)/Lib/MathLib.hpp

BUILDSYSDEPS = $(call rwildcard,$(SRCDIR)/BuildSystem,*.cpp)
SRCPSF = $(call rwildcard,$(SRCDIR)/Lib,*.psf)
SRCXX = $(call rwildcard,$(SRCDIR)/Lib,*.cpp)
SRCXX += $(SRCDIR)/Lib/Font.cpp

$(BUILDDIR)/libMath.so: $(BUILDDIR)/LibStub.o
	@$(MKDIR) $(@D)
	@$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$@ $< -o $@
	@echo "==> Created: $@"
$(BUILDDIR)/LibStub.o: $(SRCDIR)/Lib/MathLib.hpp $(HEADERS)
	@$(MKDIR) $(@D)
	@$(CXX) $(CXXFLAGS) -fPIC -x c++ -c $< -o $@
	@echo "==> Created: $@"
$(SRCDIR)/Lib/Font.cpp: $(SRCPSF) $(SCRIPTSDIR)/PSFToCXX.py Makefile
	@$(MKDIR) $(@D)
	@$(PYTHON) $(SCRIPTSDIR)/PSFToCXX.py $@ $(SRCDIR)/Lib/Font.hpp $(SRCPSF)
	@echo "==> Created: $@"
$(SRCDIR)/Lib/MathLib.hpp: $(SCRIPTSDIR)/MakeIncludes.py $(SRCXX) Makefile
	@$(MKDIR) $(@D)
	@$(PYTHON) $< $(patsubst $(SRCDIR)/Lib/%.cpp, %.cpp, $(SRCXX)) $@
	@echo "==> Created: $@"
$(BUILDDIR)/TmpBuild.out: $(BUILDSYSDEPS) $(HEADERS) $(BUILDDIR)/libMath.so
	@$(MKDIR) $(@D)
	@$(CXX) $(CXXFLAGS) $(BUILDSYSDEPS) -o $@ -L $(BUILDDIR) -l Math
	@echo "==> Created: $@"
$(BUILDDIR)/Build.mk: $(BUILDDIR)/TmpBuild.out Build.txt
	@$(MKDIR) $(@D)
	@./$^ $@
	@echo "==> Created: $@"

MATHPROGRAMS ?= $(call rwildcard,$(SRCDIR)/TestPrograms/Math,*.txt)
MLITERS ?= 20000
SERVERPORT ?= 8080
INTERPRETERPROGRAM ?= $(SRCDIR)/TestPrograms/Interpreter/Main.txt
CHEMPROGRAM ?= $(SRCDIR)/TestPrograms/Chemistry/All.txt
CARBONS ?= 6
ANIMATIONFILE ?= $(SRCDIR)/TestPrograms/Math/Trigonometry/Sine.txt
POLYNOMIAL ?= $(SRCDIR)/TestPrograms/Math/Polynomial.txt
IMGCONVINPUTTYPE ?= TGA
IMGCONVINPUT ?= $(SRCDIR)/TestPrograms/RPG/Emoji.tga
IMGCONVOUTPUTTYPE ?= P6
IMGCONVOUTPUT ?= $(BUILDDIR)/ImageConverter.ppm
ELFPROGRAM ?= $(BUILDDIR)/ELFParser.out
4DPROGRAM ?= $(SRCDIR)/TestPrograms/4D/Tesseract.txt
HEADERTREEINPUT ?= $(SRCDIR)/Lib/Host.hpp
FSCONVFS ?= GNUTAR
FSCONVOUTPUT ?= $(BUILDDIR)/FS.tar
FSCONVINPUT ?= Makefile Makefile TODO.md TODO.md
BENCODEINPUT ?= $(SRCDIR)/TestPrograms/Bencode/ArchLinux.torrent
LAMBDAINPUT ?= $(SRCDIR)/TestPrograms/LambdaCalculus/Main.txt
SCRAPERINPUT ?= $(SRCDIR)/TestPrograms/WebScraper/Wikipedia.json
SCRAPEROUTPUT ?= $(BUILDDIR)/WebScraperOutput
SEQUENTINPUT ?= $(SRCDIR)/TestPrograms/SequentCalculus/ModusPonens.txt
PROVERINPUT ?= $(SRCDIR)/TestPrograms/Prover/Main.txt
3DVIEWERINPUT ?= $(SRCDIR)/TestPrograms/SlotMachine/Machine.obj
3DVIEWERINPUTTYPE ?= WavefrontObject
TYPETHEORYINPUT ?= $(SRCDIR)/TestPrograms/TypeTheory/Main.txt
SUDOKUINPUT ?= $(SRCDIR)/TestPrograms/Sudoku/2.csv
VIDEOPLAYERWIDTH ?= 800
VIDEOPLAYERHEIGHT ?= 800
VIDEOPLAYERSPEED ?= 0.1
VIDEOPLAYERSKIPDURATION ?= 1
VIDEOPLAYERMULTX ?= 4
VIDEOPLAYERMULTY ?= 4
VIDEOPLAYERPATH ?= $(SRCDIR)/TestPrograms/VideoPlayer/Video.aseprite
AESTESTSPATH ?= $(SRCDIR)/TestPrograms/AES
OSROOT ?= $(SRCDIR)/TestPrograms/OS
OSCXX = x86_64-elf-$(CXX)
OSCXXFLAGS = $(CXXFLAGS) -DFreestanding -ffreestanding -mcmodel=large -mno-red-zone -fno-exceptions -fno-rtti -fno-omit-frame-pointer -fstack-protector-all
OSLINKER = $(SRCDIR)/OS/Linker.ld
OSLDFLAGS = $(OSCXXFLAGS) -Bsymbolic -nostdlib
OSQEMUCMD = qemu-system-x86_64 -usb -smp 1 -M q35 -m 4096 -rtc base=localtime -boot d \
-serial file:$(BUILDDIR)/OS.log -device rtl8139,netdev=net0 -cdrom $(BUILDDIR)/OS.img \
-drive file=$(BUILDDIR)/FAT.img,format=raw,media=disk \
-netdev user,id=net0,hostfwd=tcp::$(SERVERPORT)-:$(SERVERPORT)

clean:
	@$(MKDIR) $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@touch $(SRCDIR)/Lib/MathLib.hpp
	@rm $(SRCDIR)/Lib/MathLib.hpp
	@echo "==> Deleted compiled files"
.PHONY: clean

include $(BUILDDIR)/Build.mk