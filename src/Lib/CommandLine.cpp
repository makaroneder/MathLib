#include "CommandLine.hpp"
#include "String.hpp"

namespace MathLib {
    CommandLine::CommandLine(size_t argc, const char** argv) {
        String prevName = "";
        for (size_t i = 1; i < argc; i++) {
            const String arg = argv[i];
            if (arg.At(0) == '-') {
                if (prevName.GetSize() && !entries.Add(CommandLineEntry(prevName, ""_M))) Panic("Failed to add command line entry");
                prevName = SubString(arg, 1, arg.GetSize() - 1);
            }
            else if (prevName.GetSize()) {
                if (!entries.Add(CommandLineEntry(prevName, arg))) Panic("Failed to add command line entry");
                prevName = "";
            }
            else Panic("Unknown command line entry: '"_M + arg + '\'');
        }
    }
    Expected<String> CommandLine::GetEntry(const Sequence<char>& name) const {
        for (const CommandLineEntry& entry : entries)
            if (entry.name == name) return entry.value;
        return Expected<String>();
    }
}