#ifndef MathLib_FileSystem_ExtendedFileSystem_GroupDescriptor_H
#define MathLib_FileSystem_ExtendedFileSystem_GroupDescriptor_H
#include <stdint.h>

namespace MathLib {
    struct ExtendedFileSystemGroupDescriptor {
        uint32_t blockBitmap;
        uint32_t inodeBitmap;
        uint32_t inodeTable;
        uint16_t unallocatedBlocks;
        uint16_t unallocatedInodes;
        uint16_t directoriesInGroup;
        uint16_t reserved[7];
    } __attribute__((packed));
}

#endif