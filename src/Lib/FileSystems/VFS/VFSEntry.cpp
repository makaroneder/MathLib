#include "VFSEntry.hpp"

namespace MathLib {
    VFSEntry::VFSEntry(FileSystem* fs, String name) : fs(fs), name(name) {}
}