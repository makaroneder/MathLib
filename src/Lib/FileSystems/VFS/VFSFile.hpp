#ifndef FileSystems_VFS_File_H
#define FileSystems_VFS_File_H
#include <stddef.h>
#include <stdint.h>

namespace MathLib {
    struct VFSFile {
        size_t fs;
        size_t index;
        bool free;

        VFSFile(size_t fs = SIZE_MAX, size_t index = SIZE_MAX);
    };
}

#endif