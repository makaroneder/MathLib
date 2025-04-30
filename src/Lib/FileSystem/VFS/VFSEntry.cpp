#include "VFSEntry.hpp"

namespace MathLib {
    VFSEntry::VFSEntry(FileSystem* fs, const Sequence<char>& name) : fs(fs), name(CollectionToString(name)) {
        EmptyBenchmark
    }
    bool VFSEntry::operator==(const VFSEntry& other) const {
        StartAndReturnFromBenchmark(name == other.name);
    }
    bool VFSEntry::operator!=(const VFSEntry& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}