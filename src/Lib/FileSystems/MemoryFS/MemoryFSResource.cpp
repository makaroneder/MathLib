#include "MemoryFSResource.hpp"

MemoryFSResource::MemoryFSResource(const String& name) : name(name), buffer(Array<uint8_t>()) {}