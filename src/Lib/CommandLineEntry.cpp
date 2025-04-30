#include "CommandLineEntry.hpp"

namespace MathLib {
    CommandLineEntry::CommandLineEntry(const Sequence<char>& name, const Sequence<char>& value) : name(CollectionToString(name)), value(CollectionToString(value)) {
        EmptyBenchmark
    }
    bool CommandLineEntry::operator==(const CommandLineEntry& other) const {
        StartAndReturnFromBenchmark(name == other.name && value == other.value);
    }
    bool CommandLineEntry::operator!=(const CommandLineEntry& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}