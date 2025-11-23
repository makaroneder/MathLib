#include "QuakePakFile.hpp"

namespace MathLib {
    QuakePakFile::QuakePakFile(void) : position(0), size(0), free(true) {}
    QuakePakFile::QuakePakFile(uint32_t position, uint32_t size) : position(position), size(size), free(false) {}
    bool QuakePakFile::Equals(const QuakePakFile& other) const {
        return free == other.free && position == other.position && size == other.size;
    }
}