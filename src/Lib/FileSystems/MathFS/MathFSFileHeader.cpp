#include "MathFSFileHeader.hpp"

MathFSFileHeader::MathFSFileHeader(void) {}
MathFSFileHeader::MathFSFileHeader(Type type, uint64_t lba, uint64_t size, const String& path) : type(type), lba(lba), size(size) {
    for (size_t i = 0; i < path.GetSize() && i < SizeOfArray(name); i++) name[i] = path.At(i);
    name[SizeOfArray(name) - 1] = '\0';
}