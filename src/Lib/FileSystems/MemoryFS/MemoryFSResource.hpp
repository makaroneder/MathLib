#ifndef MemoryFSResource_H
#define MemoryFSResource_H
#include <Typedefs.hpp>

struct MemoryFSResource {
    String name;
    Array<uint8_t> buffer;

    MemoryFSResource(const String& name = "");
};

#endif