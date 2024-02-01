SRCDIR = src
BUILDDIR = bin
BUILDTYPE ?= Debug

CXX = g++
AR = ar
VALGRIND = valgrind

CXXFLAGS = -Wall -Wextra -Werror -I $(SRCDIR)/Lib -DPrintNodes
ARFLAGS = -rcs
VALGRINDFLAGS = -s --leak-check=full --show-leak-kinds=all

ifeq ($(BUILDTYPE), Debug)
CXXFLAGS += -g -O0
else
CXXFLAGS += -O2
endif

HEADERS = $(shell find $(SRCDIR) -type f -name "*.hpp")
SRCXX = $(shell find $(SRCDIR)/App -type f -name "*.cpp")
OBJS = $(patsubst $(SRCDIR)/App/%.cpp, $(BUILDDIR)/App/%.o, $(SRCXX))

$(BUILDDIR)/Math.out: $(OBJS)
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $^ -o $@ -l SDL2
	@echo "==> Created: $@"
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) Makefile
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "==> Created: $@"

PROGRAM ?= $(SRCDIR)/TestPrograms/Factorial.txt

run: $(BUILDDIR)/Math.out $(PROGRAM)
	@./$^
debug: $(BUILDDIR)/Math.out $(PROGRAM)
	@$(VALGRIND) $(VALGRINDFLAGS) ./$^
clean:
	@mkdir -p $(BUILDDIR)
	@rm -rf $(BUILDDIR)/*
	@echo "==> Deleted compiled files"
.PHONY: run debug clean