#ifndef FileSystems_VFS_Entry_H
#define FileSystems_VFS_Entry_H
#include "../FileSystem.hpp"

namespace MathLib {
    struct VFSEntry {
        FileSystem* fs;
        String name;

        VFSEntry(FileSystem* fs = nullptr, String name = "");
    };
}

#endif