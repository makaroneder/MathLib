#include "VFSFile.hpp"

namespace MathLib {
    VFSFile::VFSFile(size_t fs, size_t index) : fs(fs), index(index), free(false) {}
    bool VFSFile::operator==(const VFSFile& other) const {
        return fs == other.fs && index == other.index && free == other.free;
    }
    bool VFSFile::operator!=(const VFSFile& other) const {
        return !(*this == other);
    }
}