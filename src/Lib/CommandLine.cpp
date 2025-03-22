#include "CommandLine.hpp"
#include "String.hpp"

namespace MathLib {
    CommandLineEntry::CommandLineEntry(const Sequence<char>& name, const Sequence<char>& value) : name(CollectionToString(name)), value(CollectionToString(value)) {
        EmptyBenchmark
    }
    CommandLine::CommandLine(size_t argc, const char** argv) {
        StartBenchmark
        String prevName = "";
        for (size_t i = 1; i < argc; i++) {
            const String arg = argv[i];
            if (arg.At(0) == '-') {
                if (!prevName.IsEmpty() && !entries.Add(CommandLineEntry(prevName, ""_M))) Panic("Failed to add command line entry");
                prevName = SubString(arg, 1, arg.GetSize() - 1);
            }
            else if (!prevName.IsEmpty()) {
                if (!entries.Add(CommandLineEntry(prevName, arg))) Panic("Failed to add command line entry");
                prevName = "";
            }
            else Panic("Unknown command line entry: '"_M + arg + '\'');
        }
        EndBenchmark
    }
    Expected<String> CommandLine::GetEntry(const Sequence<char>& name) const {
        StartBenchmark
        for (const CommandLineEntry& entry : entries)
            if (entry.name == name) ReturnFromBenchmark(entry.value);
        ReturnFromBenchmark(Expected<String>());
    }
}