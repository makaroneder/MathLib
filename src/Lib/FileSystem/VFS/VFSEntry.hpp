#ifndef MathLib_FileSystem_VFS_Entry_H
#define MathLib_FileSystem_VFS_Entry_H
#include "../FileSystem.hpp"

namespace MathLib {
    struct VFSEntry {
        FileSystem* fs;
        String name;

        VFSEntry(FileSystem* fs = nullptr, const Sequence<char>& name = ""_M);
        [[nodiscard]] bool operator==(const VFSEntry& other) const;
        [[nodiscard]] bool operator!=(const VFSEntry& other) const;
    };
}

#endif