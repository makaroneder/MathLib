SRCDIR = src
BUILDDIR = bin
BUILDTYPE ?= Debug

CXX = g++
AR = ar
VALGRIND = valgrind

CXXFLAGS = -Wall -Wextra -Werror -I $(SRCDIR)/Lib -I $(SRCDIR)/Platform
ARFLAGS = -rcs
VALGRINDFLAGS = -s --leak-check=full --show-leak-kinds=all

ifeq ($(BUILDTYPE), Debug)
CXXFLAGS += -g -O0 -DDebug
else
CXXFLAGS += -O2
endif

HEADERS = $(shell find $(SRCDIR) -type f -name "*.hpp")
SRCXX = $(shell find $(SRCDIR)/Lib -type f -name "*.cpp")
OBJS = $(patsubst $(SRCDIR)/Lib/%.cpp, $(BUILDDIR)/Lib/%.o, $(SRCXX))

GRAPHSRCXX = $(shell find $(SRCDIR)/Graph -type f -name "*.cpp")
GRAPHOBJS = $(patsubst $(SRCDIR)/Graph/%.cpp, $(BUILDDIR)/Graph/%.o, $(GRAPHSRCXX))

QUIZSRCXX = $(shell find $(SRCDIR)/Quiz -type f -name "*.cpp")
QUIZOBJS = $(patsubst $(SRCDIR)/Quiz/%.cpp, $(BUILDDIR)/Quiz/%.o, $(QUIZSRCXX))

SIMSRCXX = $(shell find $(SRCDIR)/Simulation -type f -name "*.cpp")
SIMOBJS = $(patsubst $(SRCDIR)/Simulation/%.cpp, $(BUILDDIR)/Simulation/%.o, $(SIMSRCXX))

CHEMSRCXX = $(shell find $(SRCDIR)/Chemistry -type f -name "*.cpp")
CHEMOBJS = $(patsubst $(SRCDIR)/Chemistry/%.cpp, $(BUILDDIR)/Chemistry/%.o, $(CHEMSRCXX))

TESTSRCXX = $(shell find $(SRCDIR)/Tests -type f -name "*.cpp")
TESTOBJS = $(patsubst $(SRCDIR)/Tests/%.cpp, $(BUILDDIR)/Tests/%.o, $(TESTSRCXX))

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

all: $(BUILDDIR)/MathGraph.out $(BUILDDIR)/Quiz.out $(BUILDDIR)/Simulation.out $(BUILDDIR)/Chemistry.out $(BUILDDIR)/Tests.out
$(BUILDDIR)/libMath.a: $(OBJS)
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $^
	@echo "==> Created: $@"
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) Makefile
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "==> Created: $@"

PROGRAMS ?= $(shell find $(SRCDIR)/TestPrograms -type f -name "*.txt")

$(eval $(call AddTarget,$(BUILDDIR)/MathGraph.out,$(GRAPHOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/Quiz.out,$(QUIZOBJS),))
$(eval $(call AddTarget,$(BUILDDIR)/Simulation.out,$(SIMOBJS),-l SDL2))
$(eval $(call AddTarget,$(BUILDDIR)/Chemistry.out,$(CHEMOBJS),))
$(eval $(call AddTarget,$(BUILDDIR)/Tests.out,$(TESTOBJS),))

$(eval $(call RunTarget,run,debug,$(BUILDDIR)/MathGraph.out,$(PROGRAMS)))
$(eval $(call RunTarget,runQuiz,debugQuiz,$(BUILDDIR)/Quiz.out,$(PROGRAMS)))
$(eval $(call RunTarget,runSim,debugSim,$(BUILDDIR)/Simulation.out,))
$(eval $(call RunTarget,runChem,debugChem,$(BUILDDIR)/Chemistry.out,))

test: $(BUILDDIR)/Tests.out
	@./$<
clean:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@echo "==> Deleted compiled files"
.PHONY: all test clean