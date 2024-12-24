#ifndef MathLib_FileSystem_MemoryFS_File_H
#define MathLib_FileSystem_MemoryFS_File_H
#include <stddef.h>
#include <stdint.h>

namespace MathLib {
    struct MemoryFSFile {
        size_t resource;
        bool write;
        bool free;

        MemoryFSFile(size_t resource = SIZE_MAX, bool write = false);
    };
}

#endif