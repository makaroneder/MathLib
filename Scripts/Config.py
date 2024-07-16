import sys

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print(f"Usage: {sys.argv[0]} <input file> <output file>")
        exit(1)
    targets = []
    targetNames = ""
    with open(sys.argv[1], 'r') as f:
        for line in f:
            line = line.strip()
            parts = line[1 : -1].split('" "')
            if len(parts) < 2:
                print(f"Error on line: {line}")
                print(f"{len(parts)}")
                exit(1)
            targetNames += " $(BUILDDIR)/" + parts[1]
            # mode, target, directory, flags, runName, arguments
            if parts[0] == "host":
                if len(parts) == 6: targets.append((parts[0], (parts[1], parts[2], parts[3], parts[4], parts[5])))
                else:
                    print(f"Too little arguments for mode {parts[0]} on line: {line}")
                    exit(1)
            # mode, target, directory, cxx, cxxFlags, as, asFlags, extraLinkerFlags, dependencies, runName, runCommand
            elif parts[0] == "freestanding":
                if len(parts) == 11: targets.append((parts[0], (parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7], parts[8], parts[9], parts[10])))
                else:
                    print(f"Too little arguments for mode {parts[0]} on line: {line}")
                    exit(1)
                pass
            else:
                print(f"Unknown mode ({parts[0]}) on line: {line}")
                exit(1)
    with open(sys.argv[2], 'w') as f:
        f.write("# target, objects, flags, dependencies, cxx, cxxFlags, as, asFlags, directory\n")
        f.write("define AddTarget\n")
        f.write("$(BUILDDIR)/Objects/$(9)/%.o: $(SRCDIR)/$(9)/%.cpp $(HEADERS) Makefile $(BUILDDIR)/Targets.mk\n")
        f.write("\t@mkdir -p $$(@D)\n")
        f.write("\t@$(5) $(6) -c $$< -o $$@\n")
        f.write("\t@echo \"==> Created: $$@\"\n")
        f.write("$(BUILDDIR)/Objects/$(9)/%Asm.o: $(SRCDIR)/$(9)/%.asm $(HEADERS) Makefile $(BUILDDIR)/Targets.mk\n")
        f.write("\t@mkdir -p $$(@D)\n")
        f.write("\t@$(7) $(8) $$< -o $$@\n")
        f.write("\t@echo \"==> Created: $$@\"\n")
        f.write("$(BUILDDIR)/Objects/$(9)/%.o: $(SRCDIR)/$(9)/%.s $(HEADERS) Makefile $(BUILDDIR)/Targets.mk\n")
        f.write("\t@mkdir -p $$(@D)\n")
        f.write("\t@$(7) $(8) $$< -o $$@\n")
        f.write("\t@echo \"==> Created: $$@\"\n")
        f.write("$(1): $(2) $(4) $(BUILDDIR)/libExtras.a\n")
        f.write("\t@mkdir -p $$(@D)\n")
        f.write("\t@$(5) $(6) $(2) -o $$@ $(3) -L $(BUILDDIR) -l Extras\n")
        f.write("\t@echo \"==> Created: $$@\"\n")
        f.write("endef\n")
        f.write("# runName, target, args\n")
        f.write("define RunHostTarget\n")
        f.write("run$(1): $(2) test\n")
        f.write("\t@./$$< $(3)\n")
        f.write("debug$(1): $(2) test\n")
        f.write("\t@$(VALGRIND) $(VALGRINDFLAGS) ./$$< $(3)\n")
        f.write(".PHONY: run$(1) debug$(1)\n")
        f.write("endef")
        for i, (mode, args) in enumerate(targets, start = 1):
            f.write("\n\n")
            if mode == "host":
                f.write(f"SRCXX{i} = $(shell find $(SRCDIR)/{args[1]} -type f -name \"*.cpp\")\n")
                f.write(f"SRCAS{i} = $(shell find $(SRCDIR)/{args[1]} -type f -name \"*.asm\")\n")
                f.write(f"OBJS{i} = $(patsubst $(SRCDIR)/{args[1]}/%.cpp, $(BUILDDIR)/Objects/{args[1]}/%.o, $(SRCXX{i}))\n")
                f.write(f"OBJS{i} += $(patsubst $(SRCDIR)/{args[1]}/%.asm, $(BUILDDIR)/Objects/{args[1]}/%Asm.o, $(SRCAS{i}))\n")
                f.write(f"$(eval $(call AddTarget,$(BUILDDIR)/{args[0]},$(OBJS{i}),{args[2]},,$(CXX),$(CXXFLAGS),$(AS),$(ASFLAGS),{args[1]}))\n")
                f.write(f"$(eval $(call RunHostTarget,{args[3]},$(BUILDDIR)/{args[0]},{args[4]}))")
            elif mode == "freestanding":
                f.write(f"SRCXX{i} = $(shell find $(SRCDIR)/{args[1]} -type f -name \"*.cpp\")\n")
                f.write(f"SRCAS{i} = $(shell find $(SRCDIR)/{args[1]} -type f -name \"*.asm\")\n")
                f.write(f"OBJS{i} = $(BUILDDIR)/Objects/{args[1]}/crti.o $(shell {args[2]} {args[3]} -print-file-name=crtbegin.o)\n")
                f.write(f"OBJS{i} += $(patsubst $(SRCDIR)/{args[1]}/%.cpp, $(BUILDDIR)/Objects/{args[1]}/%.o, $(SRCXX{i}))\n")
                f.write(f"OBJS{i} += $(patsubst $(SRCDIR)/{args[1]}/%.asm, $(BUILDDIR)/Objects/{args[1]}/%Asm.o, $(SRCAS{i}))\n")
                f.write(f"OBJS{i} += $(shell {args[2]} {args[3]} -print-file-name=crtend.o) $(BUILDDIR)/Objects/{args[1]}/crtn.o\n")
                f.write(f"$(eval $(call AddTarget,$(BUILDDIR)/{args[0]},$(OBJS{i}),{args[6]},{args[7]},{args[2]},{args[3]},{args[4]},{args[5]},{args[1]}))\n")
                f.write(f"run{args[8]}: $(BUILDDIR)/{args[0]}\n")
                f.write(f"\t@{args[9]}")
            else:
                print(f"Unknown mode: {mode}")
                exit(1)
        f.write(f"\nall:{targetNames}\n.PHONY: all")