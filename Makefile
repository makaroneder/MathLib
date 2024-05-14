SRCDIR = src
BUILDDIR = bin
BUILDTYPE ?= Debug

CXX = g++
AR = ar
OBJCOPY = objcopy
VALGRIND = valgrind

CXXFLAGS = -Wall -Wextra -Werror -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ARFLAGS = -rcs
OBJCOPYFLAGS = -O elf64-x86-64 -B i386 -I binary
VALGRINDFLAGS = -s --leak-check=full --show-leak-kinds=all

ifeq ($(BUILDTYPE), Debug)
CXXFLAGS += -g -O0 -DDebug
else
CXXFLAGS += -O2
endif

HEADERS = $(shell find $(SRCDIR) -type f -name "*.hpp")
HEADERS += $(shell find $(SRCDIR)/Platform -type f -name "*.cpp")

SRCXX = $(shell find $(SRCDIR)/Lib -type f -name "*.cpp")
SRCPSF = $(shell find $(SRCDIR)/Lib -type f -name "*.psf")
OBJS = $(patsubst $(SRCDIR)/Lib/%.cpp, $(BUILDDIR)/Objects/Lib/%.o, $(SRCXX))
OBJS += $(patsubst $(SRCDIR)/Lib/%.psf, $(BUILDDIR)/Objects/Lib/%.o, $(SRCPSF))

GRAPHSRCXX = $(shell find $(SRCDIR)/Graph -type f -name "*.cpp")
GRAPHOBJS = $(patsubst $(SRCDIR)/Graph/%.cpp, $(BUILDDIR)/Objects/Graph/%.o, $(GRAPHSRCXX))

QUIZSRCXX = $(shell find $(SRCDIR)/Quiz -type f -name "*.cpp")
QUIZOBJS = $(patsubst $(SRCDIR)/Quiz/%.cpp, $(BUILDDIR)/Objects/Quiz/%.o, $(QUIZSRCXX))

SIMSRCXX = $(shell find $(SRCDIR)/Simulation -type f -name "*.cpp")
SIMOBJS = $(patsubst $(SRCDIR)/Simulation/%.cpp, $(BUILDDIR)/Objects/Simulation/%.o, $(SIMSRCXX))

CHEMSRCXX = $(shell find $(SRCDIR)/Chemistry -type f -name "*.cpp")
CHEMOBJS = $(patsubst $(SRCDIR)/Chemistry/%.cpp, $(BUILDDIR)/Objects/Chemistry/%.o, $(CHEMSRCXX))

GAMESRCXX = $(shell find $(SRCDIR)/GameEngine -type f -name "*.cpp")
GAMEOBJS = $(patsubst $(SRCDIR)/GameEngine/%.cpp, $(BUILDDIR)/Objects/GameEngine/%.o, $(GAMESRCXX))

MLSRCXX = $(shell find $(SRCDIR)/MachineLearning -type f -name "*.cpp")
MLOBJS = $(patsubst $(SRCDIR)/MachineLearning/%.cpp, $(BUILDDIR)/Objects/MachineLearning/%.o, $(MLSRCXX))

3DSRCXX = $(shell find $(SRCDIR)/3D -type f -name "*.cpp")
3DOBJS = $(patsubst $(SRCDIR)/3D/%.cpp, $(BUILDDIR)/Objects/3D/%.o, $(3DSRCXX))

CRYPTSRCXX = $(shell find $(SRCDIR)/Cryptography -type f -name "*.cpp")
CRYPTOBJS = $(patsubst $(SRCDIR)/Cryptography/%.cpp, $(BUILDDIR)/Objects/Cryptography/%.o, $(CRYPTSRCXX))

GRAVSRCXX = $(shell find $(SRCDIR)/Gravity -type f -name "*.cpp")
GRAVOBJS = $(patsubst $(SRCDIR)/Gravity/%.cpp, $(BUILDDIR)/Objects/Gravity/%.o, $(GRAVSRCXX))

CMPSRCXX = $(shell find $(SRCDIR)/ComplexAnimation -type f -name "*.cpp")
CMPOBJS = $(patsubst $(SRCDIR)/ComplexAnimation/%.cpp, $(BUILDDIR)/Objects/ComplexAnimation/%.o, $(CMPSRCXX))

TESTSRCXX = $(shell find $(SRCDIR)/Tests -type f -name "*.cpp")
TESTOBJS = $(patsubst $(SRCDIR)/Tests/%.cpp, $(BUILDDIR)/Objects/Tests/%.o, $(TESTSRCXX))

define AddTarget
$(1): $(2) $(BUILDDIR)/libMath.a
	@mkdir -p $(BUILDDIR)
	@$(CXX) $(CXXFLAGS) $(2) -o $(1) $(3) -L $(BUILDDIR) -l Math
	@echo "==> Created: $(1)"
endef
define RunTarget
$(1): $(3) $(4) test
	@./$(3) $(4)
$(2): $(3) $(4) test
	@$(VALGRIND) $(VALGRINDFLAGS) ./$(3) $(4)
.PHONY: $(1) $(2)
endef

$(BUILDDIR)/libMath.a: $(OBJS)
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $^
	@echo "==> Created: $@"
$(BUILDDIR)/Objects/%.o: $(SRCDIR)/%.cpp $(HEADERS) Makefile
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "==> Created: $@"
$(BUILDDIR)/Objects/%.o: $(SRCDIR)/%.psf Makefile
	@mkdir -p $(@D)
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
	@echo "==> Created: $@"

MATHPROGRAMS ?= $(shell find $(SRCDIR)/TestPrograms/Math -type f -name "*.txt")
CRYPTMSG ?= $(SRCDIR)/TestPrograms/Cryptography/CaesarCipher.txt

$(eval $(call AddTarget,$(BUILDDIR)/MathGraph.out,$(GRAPHOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/Quiz.out,$(QUIZOBJS),))
$(eval $(call AddTarget,$(BUILDDIR)/Simulation.out,$(SIMOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/Chemistry.out,$(CHEMOBJS),))
$(eval $(call AddTarget,$(BUILDDIR)/GameEngine.out,$(GAMEOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/MachineLearning.out,$(MLOBJS),))
$(eval $(call AddTarget,$(BUILDDIR)/3D.out,$(3DOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/Cryptography.out,$(CRYPTOBJS),))
$(eval $(call AddTarget,$(BUILDDIR)/Gravity.out,$(GRAVOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/ComplexAnimation.out,$(CMPOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/Tests.out,$(TESTOBJS),))

$(eval $(call RunTarget,run,debug,$(BUILDDIR)/MathGraph.out,$(MATHPROGRAMS)))
$(eval $(call RunTarget,runQuiz,debugQuiz,$(BUILDDIR)/Quiz.out,$(MATHPROGRAMS)))
$(eval $(call RunTarget,runSim,debugSim,$(BUILDDIR)/Simulation.out,))
$(eval $(call RunTarget,runChem,debugChem,$(BUILDDIR)/Chemistry.out,))
$(eval $(call RunTarget,runEngine,debugEngine,$(BUILDDIR)/GameEngine.out,))
$(eval $(call RunTarget,run3D,debug3D,$(BUILDDIR)/3D.out,))
$(eval $(call RunTarget,runCrypt,debugCrypt,$(BUILDDIR)/Cryptography.out,$(CRYPTMSG)))
$(eval $(call RunTarget,runGrav,debugGrav,$(BUILDDIR)/Gravity.out,))

MLITERS = 20000

$(BUILDDIR)/%.bin: $(BUILDDIR)/MachineLearning.out test
	@mkdir -p $(@D)
	@./$< $@ $(MLITERS)
	@echo "==> Created: $@"
$(BUILDDIR)/%.mp4: $(BUILDDIR)/ComplexAnimation.out test
	@mkdir -p $(@D)
	@./$< $(@D) $@
	@echo "==> Created: $@"

runML: $(BUILDDIR)/ML.bin
cleanML:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/ML.bin
runAnim: $(BUILDDIR)/Animation/Final.mp4
cleanAnim:
	@mkdir -p $(BUILDDIR)/Animation
	@rm -rf $(BUILDDIR)/Animation

test: $(BUILDDIR)/Tests.out
	@./$<
clean:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@echo "==> Deleted compiled files"
.PHONY: test clean runML cleanML runAnim cleanAnim