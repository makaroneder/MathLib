#ifndef MathLib_FileSystem_MemoryFS_Resource_H
#define MathLib_FileSystem_MemoryFS_Resource_H
#include "../../Typedefs.hpp"

namespace MathLib {
    struct MemoryFSResource {
        String name;
        Array<uint8_t> buffer;

        MemoryFSResource(const Sequence<char>& name = ""_M);
    };
}

#endif