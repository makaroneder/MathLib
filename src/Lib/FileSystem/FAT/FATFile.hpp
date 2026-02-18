#ifndef MathLib_FileSystem_FAT_File_H
#define MathLib_FileSystem_FAT_File_H
#include <stdint.h>

namespace MathLib {
    struct FATFile {
        uint32_t cluster;
        uint32_t size;
        bool write : 1;
        bool free : 1;

        FATFile(void);
        FATFile(uint32_t cluster, uint32_t size, bool write);
        [[nodiscard]] bool operator==(const FATFile& other) const;
        [[nodiscard]] bool operator!=(const FATFile& other) const;
    };
}

#endif