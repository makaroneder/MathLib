#ifndef MathLib_CommandLineEntry_H
#define MathLib_CommandLineEntry_H
#include <Typedefs.hpp>

namespace MathLib {
    struct CommandLineEntry {
        String name;
        String value;

        CommandLineEntry(const Sequence<char>& name = ""_M, const Sequence<char>& value = ""_M);
        [[nodiscard]] bool operator==(const CommandLineEntry& other) const;
        [[nodiscard]] bool operator!=(const CommandLineEntry& other) const;
    };
}

#endif