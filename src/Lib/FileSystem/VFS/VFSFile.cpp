#include "VFSFile.hpp"
#include <stdint.h>

namespace MathLib {
    VFSFile::VFSFile(void) : fs(SIZE_MAX), index(SIZE_MAX), free(true) {}
    VFSFile::VFSFile(size_t fs, size_t index) : fs(fs), index(index), free(false) {}
    bool VFSFile::Equals(const VFSFile& other) const {
        return fs == other.fs && index == other.index && free == other.free;
    }
}