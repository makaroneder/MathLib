#include "ExtendedFileSystem.hpp"
#include "ExtendedFileSystemInode.hpp"
#include "ExtendedFileSystemGroupDescriptor.hpp"

namespace MathLib {
    size_t RoundUpDivision(size_t a, size_t b) {
        return a / b + !!(a % b);
    }
    ExtendedFileSystem::ExtendedFileSystem(ByteDevice& disk) : PhysicalFileSystem(disk), blockGroupCount(0) {
        if (!disk.ReadPositioned<ExtendedFileSystemSuperblock>(superblock, 1024)) Panic("Failed to read superblock");
        if (superblock.IsValid()) {
            blockGroupCount = RoundUpDivision(superblock.blockCount, superblock.blocksPerGroup);
            if (blockGroupCount != RoundUpDivision(superblock.inodeCount, superblock.inodesPerGroup)) blockGroupCount = 0;
        }
        if (true) {
            const size_t inodeNumber = 2;
            const size_t group = GetInodeBlockGroup(inodeNumber);

            const uint64_t blockSize = superblock.GetBlockSize();
            const uint16_t inodeSize = superblock.GetInodeSize();
            ExtendedFileSystemGroupDescriptor descriptor;
            if (!disk.ReadPositioned<ExtendedFileSystemGroupDescriptor>(descriptor, group * sizeof(ExtendedFileSystemGroupDescriptor) + BlockToLBA(1 + (blockSize == 1024)))) Panic("Failed to read group descriptor");
            const size_t lba = BlockToLBA(descriptor.inodeTable) + GetInodeIndex(inodeNumber) * inodeSize;

            uint8_t inodeBuff[inodeSize] = { 0, };
            if (!disk.ReadPositionedBuffer(inodeBuff, inodeSize, lba)) Panic("Failed to read inode");
            ExtendedFileSystemInode* inode = (ExtendedFileSystemInode*)inodeBuff;
            (void)inode;
            // TODO: Flags
            // TODO: Follow blockPointers
        }
    }
    size_t ExtendedFileSystem::BlockToLBA(size_t block) const {
        return block * superblock.GetBlockSize();
    }
    size_t ExtendedFileSystem::GetInodeBlockGroup(size_t inode) const {
        return (inode - 1) / superblock.inodesPerGroup;
    }
    size_t ExtendedFileSystem::GetInodeIndex(size_t inode) const {
        return (inode - 1) % superblock.inodesPerGroup;
    }
    bool ExtendedFileSystem::IsValid(void) const {
        return blockGroupCount && (superblock.majorVersion < 1 || !(superblock.compressed || superblock.replayJournal || superblock.hasJournalDevice || superblock.requiredFeatures));
    }
    bool ExtendedFileSystem::Create(void) {
        // TODO:
        return false;
    }
    size_t ExtendedFileSystem::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        // TODO:
        (void)path;
        (void)mode;
        return SIZE_MAX;
    }
    bool ExtendedFileSystem::Close(size_t file) {
        // TODO:
        (void)file;
        return false;
    }
    size_t ExtendedFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
        // TODO:
        (void)file;
        (void)buffer;
        (void)size;
        (void)position;
        return 0;
    }
    size_t ExtendedFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
        // TODO:
        (void)file;
        (void)buffer;
        (void)size;
        (void)position;
        return 0;
    }
    size_t ExtendedFileSystem::GetSize(size_t file) {
        // TODO:
        (void)file;
        return 0;
    }
    Array<FileInfo> ExtendedFileSystem::ReadDirectory(const Sequence<char>& path) {
        // TODO:
        (void)path;
        return Array<FileInfo>();
    }
    bool ExtendedFileSystem::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        // TODO:
        (void)path;
        (void)overwrite;
        return false;
    }
}