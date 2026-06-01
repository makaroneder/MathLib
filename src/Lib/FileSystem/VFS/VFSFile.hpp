#ifndef MathLib_FileSystem_VFS_File_H
#define MathLib_FileSystem_VFS_File_H
#include "../../Interfaces/Comparable.hpp"

namespace MathLib {
    struct VFSFile;
    struct VFSFile : Comparable<VFSFile> {
        size_t fs;
        size_t index;
        bool free;

        VFSFile(void);
        VFSFile(size_t fs, size_t index);

        protected:
        [[nodiscard]] virtual bool Equals(const VFSFile& other) const override;
    };
}

#endif