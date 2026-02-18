#include "MemoryFSFile.hpp"

namespace MathLib {
    MemoryFSFile::MemoryFSFile(size_t resource, bool write) : resource(resource), write(write), free(false) {}
    bool MemoryFSFile::operator==(const MemoryFSFile& other) const {
        return resource == other.resource && write == other.write && free == other.free;
    }
    bool MemoryFSFile::operator!=(const MemoryFSFile& other) const {
        return !(*this == other);
    }
}