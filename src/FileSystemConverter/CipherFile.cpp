#include "CipherFile.hpp"

CipherFile::CipherFile(size_t position, size_t size, const MathLib::Sequence<char>& path, MathLib::OpenMode mode) : path(MathLib::CollectionToString(path)), position(position), size(size), mode(mode), free(false) {}
bool CipherFile::operator==(const CipherFile& other) const {
    return path == other.path && position == other.position && size == other.size && mode == other.mode && free == other.free;
}
bool CipherFile::operator!=(const CipherFile& other) const {
    return !(*this == other);
}