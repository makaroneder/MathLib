SRCDIR = src
BUILDDIR = bin
BUILDTYPE ?= Debug

CXX = g++
AR = ar
VALGRIND = valgrind

CXXFLAGS = -Wall -Wextra -Werror -I $(SRCDIR)/Lib
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

TESTSRCXX = $(shell find $(SRCDIR)/Tests -type f -name "*.cpp")
TESTOBJS = $(patsubst $(SRCDIR)/Tests/%.cpp, $(BUILDDIR)/Tests/%.o, $(TESTSRCXX))

$(BUILDDIR)/libMath.a: $(OBJS)
	@mkdir -p $(@D)
	@$(AR) $(ARFLAGS) $@ $^
	@echo "==> Created: $@"
$(BUILDDIR)/MathGraph.out: $(GRAPHOBJS) $(BUILDDIR)/libMath.a
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(GRAPHOBJS) -o $@ -l SDL2 -L $(@D) -l Math
	@echo "==> Created: $@"
$(BUILDDIR)/Quiz.out: $(QUIZOBJS) $(BUILDDIR)/libMath.a
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(QUIZOBJS) -o $@ -L $(@D) -l Math
	@echo "==> Created: $@"
$(BUILDDIR)/Tests.out: $(TESTOBJS) $(BUILDDIR)/libMath.a
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(TESTOBJS) -o $@ -L $(@D) -l Math
	@echo "==> Created: $@"
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) Makefile
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "==> Created: $@"

PROGRAMS ?= $(shell find $(SRCDIR)/TestPrograms -type f -name "*.txt")

all: $(BUILDDIR)/MathGraph.out $(BUILDDIR)/Quiz.out $(BUILDDIR)/Tests.out
test: $(BUILDDIR)/Tests.out
	@./$<
run: $(BUILDDIR)/MathGraph.out $(PROGRAMS) test
	@./$< $(PROGRAMS)
runQuiz: $(BUILDDIR)/Quiz.out $(PROGRAMS) test
	@./$< $(PROGRAMS)
debug: $(BUILDDIR)/MathGraph.out $(PROGRAMS) test
	@$(VALGRIND) $(VALGRINDFLAGS) ./$< $(PROGRAMS)
debugQuiz: $(BUILDDIR)/Quiz.out $(PROGRAMS) test
	@$(VALGRIND) $(VALGRINDFLAGS) ./$< $(PROGRAMS)
clean:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@echo "==> Deleted compiled files"
.PHONY: test run runQuiz debug debugQuiz clean