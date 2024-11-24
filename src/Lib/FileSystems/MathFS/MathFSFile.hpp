#ifndef FileSystems_MathFS_FIle_H
#define FileSystems_MathFS_FIle_H
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