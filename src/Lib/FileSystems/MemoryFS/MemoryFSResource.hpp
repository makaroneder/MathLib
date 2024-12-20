#ifndef FileSystems_MemoryFS_Resource_H
#define FileSystems_MemoryFS_Resource_H
#include <Typedefs.hpp>

namespace MathLib {
    struct MemoryFSResource {
        String name;
        Array<uint8_t> buffer;

        MemoryFSResource(const String& name = "");
    };
}

#endif