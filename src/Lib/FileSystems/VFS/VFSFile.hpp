#ifndef VFSFile_H
#define VFSFile_H
#include <stddef.h>
#include <stdint.h>

struct VFSFile {
    size_t fs;
    size_t index;
    bool free;

    VFSFile(size_t fs = SIZE_MAX, size_t index = SIZE_MAX);
};

#endif