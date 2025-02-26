#ifndef MathLib_CommandLine_H
#define MathLib_CommandLine_H
#include "Expected.hpp"

namespace MathLib {
    struct CommandLineEntry {
        String name;
        String value;

        CommandLineEntry(const String& name = "", const String& value = "");
    };
    struct CommandLine {
        CommandLine(size_t argc, const char** argv);
        [[nodiscard]] Expected<String> GetEntry(const String& name) const;

        private:
        Array<CommandLineEntry> entries;
    };
}

#endif