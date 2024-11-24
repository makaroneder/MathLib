#include "VFSFile.hpp"

namespace MathLib {
    VFSFile::VFSFile(size_t fs, size_t index) : fs(fs), index(index), free(false) {}
}