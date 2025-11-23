#ifndef MathLib_FileSystem_ExtendedFileSystem_Inode_H
#define MathLib_FileSystem_ExtendedFileSystem_Inode_H
#include <stdint.h>

namespace MathLib {
    struct ExtendedFileSystemInode {
        enum class Type : uint8_t {
            FIFO = 0x1,
            CharacterDevice,
            Directory = 0x4,
            BlockDevice = 0x6,
            File = 0x8,
            SymbolicLink = 0xa,
            UnixSocket = 0xc,
        };
        bool otherExecute : 1;
        bool otherWrite : 1;
        bool otherRead : 1;
        bool groupExecute : 1;
        bool groupWrite : 1;
        bool groupRead : 1;
        bool userExecute : 1;
        bool userWrite : 1;
        bool userRead : 1;
        bool sticky : 1;
        bool setGroupID : 1;
        bool setUserID : 1;
        uint8_t type : 4;
        uint16_t userID;
        uint32_t lowerSize;
        uint32_t lastAccessTime;
        uint32_t creationTime;
        uint32_t lastModificationTime;
        uint32_t deletionTime;
        uint16_t groupID;
        uint16_t hardLinkCount;
        uint32_t sectorCount;
        uint32_t flags; // TODO:
        uint32_t hurdTranslator;
        uint32_t directBlockPointer[12];
        uint32_t singlyIndirectBlockPointer;
        uint32_t doublyIndirectBlockPointer;
        uint32_t triplyIndirectBlockPointer;
        uint32_t generationNumber;
        uint32_t extendedAttributeBlock;
        uint32_t upperSize;
        uint32_t blockAddress;
        uint8_t fragmentNumber;
        uint8_t fragmentSize;
        uint16_t upperPermissions;
        uint16_t upperUserID;
        uint16_t upperGroupID;
        uint32_t authorUserID;
    } __attribute__((packed));
}

#endif