#ifndef FileSystems_MemoryFS_File_H
#define FileSystems_MemoryFS_File_H
#include <stddef.h>
#include <stdint.h>

struct MemoryFSFile {
    size_t resource;
    bool write;
    bool free;

    MemoryFSFile(size_t resource = SIZE_MAX, bool write = false);
};

#endif