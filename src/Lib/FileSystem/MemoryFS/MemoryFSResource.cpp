#include "MemoryFSResource.hpp"

namespace MathLib {
    MemoryFSResource::MemoryFSResource(const Sequence<char>& name) : name(CollectionToString(name)), buffer() {
        EmptyBenchmark
    }
}