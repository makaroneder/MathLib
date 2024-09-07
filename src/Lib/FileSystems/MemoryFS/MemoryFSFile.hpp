#ifndef MemoryFSFile_H
#define MemoryFSFile_H
#include <stddef.h>
#include <stdint.h>

struct MemoryFSFile {
    size_t resource;
    bool write;
    bool free;

    MemoryFSFile(const size_t& resource = SIZE_MAX, const bool& write = false);
};

#endif