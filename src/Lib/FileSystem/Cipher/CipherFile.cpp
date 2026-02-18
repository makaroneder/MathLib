#include "CipherFile.hpp"

namespace MathLib {
    CipherFile::CipherFile(size_t position, size_t size, const Sequence<char>& path, OpenMode mode) : path(CollectionToString(path)), position(position), size(size), mode(mode), free(false) {}
    bool CipherFile::operator==(const CipherFile& other) const {
        return path == other.path && position == other.position && size == other.size && mode == other.mode && free == other.free;
    }
    bool CipherFile::operator!=(const CipherFile& other) const {
        return !(*this == other);
    }
}