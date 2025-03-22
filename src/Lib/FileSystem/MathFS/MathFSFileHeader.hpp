#ifndef MathLib_FileSystem_MathFS_FileHeader_H
#define MathLib_FileSystem_MathFS_FileHeader_H
#include "../../Typedefs.hpp"

namespace MathLib {
    struct MathFSFileHeader {
        enum class Type : uint8_t {
            File,
            Directory,
            LastEntry,
        } type;
        uint64_t lba;
        uint64_t size;
        char name[495];

        MathFSFileHeader(void);
        MathFSFileHeader(Type type, uint64_t lba, uint64_t size, const Sequence<char>& path);
    } __attribute__((packed));
}

#endif