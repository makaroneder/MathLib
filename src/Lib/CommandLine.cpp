#include "CommandLine.hpp"
#include "String.hpp"

namespace MathLib {
    CommandLineEntry::CommandLineEntry(const String& name, const String& value) : name(name), value(value) {
        EmptyBenchmark
    }
    CommandLine::CommandLine(size_t argc, const char** argv) {
        StartBenchmark
        String prevName = "";
        for (size_t i = 1; i < argc; i++) {
            const String arg = argv[i];
            if (arg.At(0) == '-') {
                if (!prevName.IsEmpty() && !entries.Add(CommandLineEntry(prevName, ""))) Panic("Failed to add command line entry");
                prevName = SubString(arg, 1, arg.GetSize() - 1);
            }
            else if (!prevName.IsEmpty()) {
                if (!entries.Add(CommandLineEntry(prevName, arg))) Panic("Failed to add command line entry");
                prevName = "";
            }
            else Panic(String("Unknown command line entry: '") + arg + '\'');
        }
        EndBenchmark
    }
    Expected<String> CommandLine::GetEntry(const String& name) const {
        StartBenchmark
        for (const CommandLineEntry& entry : entries)
            if (entry.name == name) ReturnFromBenchmark(Expected<String>(entry.value));
        ReturnFromBenchmark(Expected<String>());
    }
}