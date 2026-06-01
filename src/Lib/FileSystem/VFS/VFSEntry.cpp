#include "VFSEntry.hpp"

namespace MathLib {
    VFSEntry::VFSEntry(void) : fs(nullptr), mountpoint(), removed(true) {}
    VFSEntry::VFSEntry(FileSystem* fs, const Sequence<char>& mountpoint) : fs(fs), mountpoint(CollectionToString(mountpoint)), removed(false) {}
    bool VFSEntry::Equals(const VFSEntry& other) const {
        return mountpoint == other.mountpoint;
    }
}