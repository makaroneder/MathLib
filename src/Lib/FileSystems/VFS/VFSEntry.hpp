#ifndef FileSystems_VFS_Entry_H
#define FileSystems_VFS_Entry_H
#include "../FileSystem.hpp"

struct VFSEntry {
    FileSystem* fs;
    String name;

    VFSEntry(FileSystem* fs = nullptr, String name = "");
};

#endif