#ifndef MathLib_CommandLine_H
#define MathLib_CommandLine_H
#include "CommandLineEntry.hpp"
#include "Expected.hpp"

namespace MathLib {
    struct CommandLine {
        CommandLine(size_t argc, const char** argv);
        [[nodiscard]] Expected<String> GetEntry(const Sequence<char>& name) const;

        private:
        Array<CommandLineEntry> entries;
    };
}

#endif