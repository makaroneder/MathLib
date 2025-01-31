#ifndef MathLib_FileSystem_FAT_File_H
#define MathLib_FileSystem_FAT_File_H
#include <stdint.h>

namespace MathLib {
    struct FATFile {
        uint32_t cluster;
        uint32_t size;
        bool write;
        bool free;

        FATFile(void);
        FATFile(uint32_t cluster, uint32_t size, bool write);
    };
}

#endif