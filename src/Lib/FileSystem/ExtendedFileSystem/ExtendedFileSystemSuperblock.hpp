#ifndef MathLib_FileSystem_ExtendedFileSystem_Superblock_H
#define MathLib_FileSystem_ExtendedFileSystem_Superblock_H
#include <stdint.h>

namespace MathLib {
    struct ExtendedFileSystemSuperblock {
        static constexpr uint16_t expectedSignature = 0xef53;
        enum class State : uint16_t {
            Okay = 1,
            Error,
        };
        enum class ErrorAction : uint16_t {
            Ignore = 1,
            MakeReadOnly,
            Panic,
        };
        enum class OperatingSystemID : uint32_t {
            Linux = 0,
            GNUHURD,
            MASIX,
            FreeBSD,
            BSDLite,
        };
        uint32_t inodeCount;
        uint32_t blockCount;
        uint32_t superuserBlockCount;
        uint32_t unallocatedBlockCount;
        uint32_t unallocatedInodeCount;
        uint32_t firstBlock;
        uint32_t blockSizeLog;
        uint32_t fragmentSizeLog;
        uint32_t blocksPerGroup;
        uint32_t fragmentsPerGroup;
        uint32_t inodesPerGroup;
        uint32_t lastMountTime;
        uint32_t lastWriteTime;
        uint16_t mountsSinceConsistencyCheck;
        uint16_t allowedMountsSinceConsistencyCheck;
        uint16_t signature;
        State state;
        ErrorAction errorAction;
        uint16_t minorVersion;
        uint32_t lastConsistencyCheckTime;
        uint32_t timeBetweenForcedConsistencyChecks;
        OperatingSystemID operatingSystemID;
        uint32_t majorVersion;
        uint16_t superuserID;
        uint16_t superuserGroupID;
        uint32_t firstFreeInode;
        uint16_t inodeSize;
        uint16_t blockGroupContainingSuperblock;
        bool preallocateBlocks : 1;
        bool afsServerInode : 1;
        bool journalExists : 1;
        bool extendedInodeAttributes : 1;
        bool resizableFileSystem : 1;
        bool hashIndexedDirectories : 1;
        uint32_t optionalFeatures : 26;
        bool compressed : 1;
        bool directoriesWithTypes : 1;
        bool replayJournal : 1;
        bool hasJournalDevice : 1;
        uint32_t requiredFeatures : 28;
        bool sparseSuperblocksAndGroupDescriptorTables : 1;
        bool extendedFileSize : 1;
        bool directoryContentsInBinaryTrees : 1;
        uint32_t readOnlyFeatures : 29;
        uint64_t fileSystemID[2];
        char volumeName[16];
        char lastMountPath[64];
        uint32_t compression;
        uint8_t blockCountToPreallocateForFiles;
        uint8_t blockCountToPreallocateForDirectories;
        uint16_t reserved1;
        uint64_t journalID[2];
        uint32_t journalInode;
        uint32_t journalDevice;
        uint32_t orphanInodeListHead;
        uint32_t reserved2[197];

        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] uint32_t GetFirstFreeInode(void) const;
        [[nodiscard]] uint16_t GetInodeSize(void) const;
        [[nodiscard]] uint64_t GetBlockSize(void) const;
        [[nodiscard]] uint64_t GetFragmentSize(void) const;
    } __attribute__((packed));
}

#endif