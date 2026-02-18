#include "VFSEntry.hpp"

namespace MathLib {
    VFSEntry::VFSEntry(FileSystem* fs, const Sequence<char>& name) : fs(fs), name(CollectionToString(name)) {}
    bool VFSEntry::operator==(const VFSEntry& other) const {
        return name == other.name;
    }
    bool VFSEntry::operator!=(const VFSEntry& other) const {
        return !(*this == other);
    }
}