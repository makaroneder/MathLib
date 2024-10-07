#include "CommandLine.hpp"
#include "String.hpp"

CommandLineEntry::CommandLineEntry(const String& name, const String& value) : name(name), value(value) {}
CommandLine::CommandLine(size_t argc, const char** argv) {
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
}
Expected<String> CommandLine::GetEntry(const String& name) const {
    for (const CommandLineEntry& entry : entries)
        if (entry.name == name) return Expected<String>(entry.value);
    return Expected<String>();
}