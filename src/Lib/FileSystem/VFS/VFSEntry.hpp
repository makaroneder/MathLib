#ifndef MathLib_FileSystem_VFS_Entry_H
#define MathLib_FileSystem_VFS_Entry_H
#include "../FileSystem.hpp"

namespace MathLib {
    struct VFSEntry;
    struct VFSEntry : Comparable<VFSEntry> {
        FileSystem* fs;
        String mountpoint;
        bool removed;

        VFSEntry(void);
        VFSEntry(FileSystem* fs, const Sequence<char>& mountpoint);

        protected:
        [[nodiscard]] virtual bool Equals(const VFSEntry& other) const override;
    };
}

#endif