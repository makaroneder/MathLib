#include "CommandLineEntry.hpp"

namespace MathLib {
    CommandLineEntry::CommandLineEntry(const Sequence<char>& name, const Sequence<char>& value) : name(CollectionToString(name)), value(CollectionToString(value)) {}
    bool CommandLineEntry::operator==(const CommandLineEntry& other) const {
        return name == other.name && value == other.value;
    }
    bool CommandLineEntry::operator!=(const CommandLineEntry& other) const {
        return !(*this == other);
    }
}