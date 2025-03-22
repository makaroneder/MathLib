#ifndef MathLib_CommandLine_H
#define MathLib_CommandLine_H
#include "Expected.hpp"

namespace MathLib {
    struct CommandLineEntry {
        String name;
        String value;

        CommandLineEntry(const Sequence<char>& name = ""_M, const Sequence<char>& value = ""_M);
    };
    struct CommandLine {
        CommandLine(size_t argc, const char** argv);
        [[nodiscard]] Expected<String> GetEntry(const Sequence<char>& name) const;

        private:
        Array<CommandLineEntry> entries;
    };
}

#endif