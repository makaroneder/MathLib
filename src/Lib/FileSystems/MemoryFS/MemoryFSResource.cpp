#include "MemoryFSResource.hpp"

namespace MathLib {
    MemoryFSResource::MemoryFSResource(const String& name) : name(name), buffer(Array<uint8_t>()) {}
}