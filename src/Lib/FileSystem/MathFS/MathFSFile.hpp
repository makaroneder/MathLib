#ifndef MathLib_FileSystem_MathFS_File_H
#define MathLib_FileSystem_MathFS_File_H
#include "MathFSFileHeader.hpp"

namespace MathLib {
    struct MathFSFile {
        MathFSFileHeader entry;
        bool write;
        bool free;

        MathFSFile(const MathFSFileHeader& entry = MathFSFileHeader(), bool write = false);
    };
}

#endif