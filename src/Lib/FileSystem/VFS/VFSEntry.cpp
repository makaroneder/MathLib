#include "VFSEntry.hpp"

namespace MathLib {
    VFSEntry::VFSEntry(FileSystem* fs, const Sequence<char>& name) : fs(fs), name(CollectionToString(name)) {
        EmptyBenchmark
    }
}