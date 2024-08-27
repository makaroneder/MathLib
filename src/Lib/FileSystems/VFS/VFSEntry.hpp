#ifndef VFSEntry_H
#define VFSEntry_H
#include "../FileSystem.hpp"

struct VFSEntry {
    FileSystem* fs;
    String name;

    VFSEntry(FileSystem* fs = nullptr, String name = "");
};

#endif