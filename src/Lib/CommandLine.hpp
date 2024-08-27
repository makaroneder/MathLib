#ifndef CommandLine_H
#define CommandLine_H
#include "Expected.hpp"

struct CommandLineEntry {
    String name;
    String value;

    CommandLineEntry(const String& name = "", const String& value = "");
};
struct CommandLine {
    CommandLine(const size_t& argc, const char** argv);
    Expected<String> GetEntry(const String& name) const;

    private:
    Array<CommandLineEntry> entries;
};

#endif