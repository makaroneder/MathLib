#ifndef MathLib_FileSystem_ExtendedFileSystem_DirectoryEntry_H
#define MathLib_FileSystem_ExtendedFileSystem_DirectoryEntry_H
#include <stdint.h>

namespace MathLib {
    struct ExtendedFileSystemDirectoryEntry {
        enum class TypeIndicator : uint8_t {
            Unknown,
            File,
            Directory,
            CharacterDevice,
            BlockDevice,
            FIFO,
            Socket,
            SymbolicLink,
        };
        uint32_t inode;
        uint16_t size;
        union {
            uint16_t nameLength16;
            struct {
                uint8_t nameLength8;
                TypeIndicator typeIndicator;
            } __attribute__((packed));
        };
        char name[];
    } __attribute__((packed));
}

#endif