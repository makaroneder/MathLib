#include "MemoryFSResource.hpp"

namespace MathLib {
    MemoryFSResource::MemoryFSResource(const Sequence<char>& name) : name(CollectionToString(name)), buffer() {
        EmptyBenchmark
    }
    bool MemoryFSResource::operator==(const MemoryFSResource& other) const {
        StartAndReturnFromBenchmark(name == other.name && buffer == other.buffer);
    }
    bool MemoryFSResource::operator!=(const MemoryFSResource& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}