#include "MemoryFSResource.hpp"

namespace MathLib {
    MemoryFSResource::MemoryFSResource(const Sequence<char>& name) : name(CollectionToString(name)), buffer() {}
    bool MemoryFSResource::operator==(const MemoryFSResource& other) const {
        return name == other.name && buffer == other.buffer;
    }
    bool MemoryFSResource::operator!=(const MemoryFSResource& other) const {
        return !(*this == other);
    }
}