#include "../Path.hpp"
#include "ExtendedFileSystem.hpp"
#include "ExtendedFileSystemDirectoryEntry.hpp"
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
    }
    size_t ExtendedFileSystem::GetInode(const Sequence<char>& path_) {
        size_t ret = 2;
        String path = CollectionToString(path_);
        ExtendedFileSystemDirectoryEntry dir;
        while (true) {
            const SingleTypePair<String> split = PopFirstPathElement(path);
            if (split.first.IsEmpty()) return ret;
            const ExtendedFileSystemInode inode = GetInodeStruct(ret);
            if (inode.type != (uint8_t)ExtendedFileSystemInode::Type::Directory) return 0;
            const uint64_t size = inode.GetSize(superblock.majorVersion >= 1 && superblock.extendedFileSize);
            uint64_t off = 0;
            bool found = false;
            while (off < size) {
                if (ReadInode(inode, &dir, sizeof(ExtendedFileSystemDirectoryEntry), off) != sizeof(ExtendedFileSystemDirectoryEntry)) return 0;
                if (dir.size < sizeof(ExtendedFileSystemDirectoryEntry)) return 0;
                if (off + dir.size > size) return 0;
                if (!dir.inode) {
                    off += dir.size;
                    continue;
                }
                const uint16_t nameLength = dir.GetNameLength(superblock.majorVersion >= 1 && superblock.directoriesWithTypes);
                char buff[nameLength + 1] = { '\0', };
                if (ReadInode(inode, buff, nameLength, off + sizeof(ExtendedFileSystemDirectoryEntry)) != nameLength) return 0;
                off += dir.size;
                if (split.first == buff) {
                    ret = dir.inode;
                    found = true;
                    break;
                }
            }
            if (!found) return 0;
            path = split.second;
        }
    }
    ExtendedFileSystemInode ExtendedFileSystem::GetInodeStruct(size_t inode) {
        const size_t group = GetInodeBlockGroup(inode);
        const uint64_t blockSize = superblock.GetBlockSize();
        const uint16_t inodeSize = superblock.GetInodeSize();
        ExtendedFileSystemGroupDescriptor descriptor;
        if (!disk.ReadPositioned<ExtendedFileSystemGroupDescriptor>(descriptor, group * sizeof(ExtendedFileSystemGroupDescriptor) + BlockToLBA(1 + (blockSize == 1024)))) Panic("Failed to read group descriptor");
        const size_t lba = BlockToLBA(descriptor.inodeTable) + GetInodeIndex(inode) * inodeSize;
        uint8_t buff[inodeSize] = { 0, };
        if (!disk.ReadPositionedBuffer(buff, inodeSize, lba)) Panic("Failed to read inode");
        return *(const ExtendedFileSystemInode*)buff;
    }
    size_t ExtendedFileSystem::ReadInode(const ExtendedFileSystemInode& inode, void* buffer, size_t size, size_t position) {
        if (position >= inode.GetSize(superblock.majorVersion >= 1 && superblock.extendedFileSize)) return 0;
        if (position + size > inode.GetSize(superblock.majorVersion >= 1 && superblock.extendedFileSize)) size = inode.GetSize(superblock.majorVersion >= 1 && superblock.extendedFileSize) - position;
        const uint64_t blockSize = superblock.GetBlockSize();
        const size_t offset = position % blockSize;
        const uint32_t block = GetBlockFromInode(inode, position / blockSize);
        if (!block) return 0;
        const size_t start = BlockToLBA(block);
        if (offset + size <= blockSize) return disk.ReadPositionedSizedBuffer(buffer, size, start + offset);
        const size_t tmp = blockSize - offset;
        return disk.ReadPositionedSizedBuffer(buffer, tmp, start + offset) + ReadInode(inode, (void*)((uintptr_t)buffer + tmp), size - tmp, position + tmp);
    }
    uint32_t ExtendedFileSystem::GetBlockFromInode(const ExtendedFileSystemInode& inode, size_t block) {
        const uint64_t blockSize = superblock.GetBlockSize();
        const uint64_t pointersPerBlock = blockSize / sizeof(uint32_t);
        if (block < 12) return inode.directBlockPointer[block];
        block -= 12;
        if (block < pointersPerBlock)
            return inode.singlyIndirectBlockPointer ? disk.ReadPositioned<uint32_t>(BlockToLBA(inode.singlyIndirectBlockPointer) + block * sizeof(uint32_t)).GetOr(0) : 0;
        block -= pointersPerBlock;
        if (block < pointersPerBlock * pointersPerBlock) {
            if (!inode.doublyIndirectBlockPointer) return 0;
            const uint32_t i = block / pointersPerBlock;
            const uint32_t j = block % pointersPerBlock;
            const uint32_t tmp = disk.ReadPositioned<uint32_t>(BlockToLBA(inode.doublyIndirectBlockPointer) + i * sizeof(uint32_t)).GetOr(0);
            if (!tmp) return 0;
            return disk.ReadPositioned<uint32_t>(BlockToLBA(tmp) + j * sizeof(uint32_t)).GetOr(0);
        }
        block -= pointersPerBlock * pointersPerBlock;
        if (block < pointersPerBlock * pointersPerBlock * pointersPerBlock) {
            if (!inode.triplyIndirectBlockPointer) return 0;
            const uint32_t i = block / (pointersPerBlock * pointersPerBlock);
            const uint32_t j = (block / pointersPerBlock) % pointersPerBlock;
            const uint32_t k = block % pointersPerBlock;
            const uint32_t tmp1 = disk.ReadPositioned<uint32_t>(BlockToLBA(inode.triplyIndirectBlockPointer) + i * sizeof(uint32_t)).GetOr(0);
            if (!tmp1) return 0;
            const uint32_t tmp2 = disk.ReadPositioned<uint32_t>(BlockToLBA(tmp1) + j * sizeof(uint32_t)).GetOr(0);
            if (!tmp2) return 0;
            return disk.ReadPositioned<uint32_t>(BlockToLBA(tmp2) + k * sizeof(uint32_t)).GetOr(0);
        }
        return 0;
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
        if (mode == OpenMode::Write) return SIZE_MAX;
        const size_t inode = GetInode(path);
        if (!inode || GetInodeStruct(inode).type != (uint8_t)ExtendedFileSystemInode::Type::File) return SIZE_MAX;
        const size_t size = files.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (files.AtUnsafe(i)) continue;
            files.AtUnsafe(i) = inode;
            return i;
        }
        return files.Add(inode) ? size : SIZE_MAX;
    }
    bool ExtendedFileSystem::Close(size_t file) {
        if (file >= files.GetSize() || !files.AtUnsafe(file)) return false;
        files.AtUnsafe(file) = 0;
        return true;
    }
    size_t ExtendedFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
        return file < files.GetSize() && files.AtUnsafe(file) ? ReadInode(GetInodeStruct(files.AtUnsafe(file)), buffer, size, position) : 0;
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
        return file < files.GetSize() && files.AtUnsafe(file) ? GetInodeStruct(files.AtUnsafe(file)).GetSize(superblock.majorVersion >= 1 && superblock.extendedFileSize) : 0;
    }
    Array<FileInfo> ExtendedFileSystem::ReadDirectory(const Sequence<char>& path) {
        const size_t tmp = GetInode(path);
        if (!tmp) return Array<FileInfo>();
        const ExtendedFileSystemInode inode = GetInodeStruct(tmp);
        if (inode.type != (uint8_t)ExtendedFileSystemInode::Type::Directory) return Array<FileInfo>();
        const uint64_t size = inode.GetSize(superblock.majorVersion >= 1 && superblock.extendedFileSize);
        ExtendedFileSystemDirectoryEntry dir;
        uint64_t off = 0;
        Array<FileInfo> ret;
        while (off < size) {
            if (ReadInode(inode, &dir, sizeof(ExtendedFileSystemDirectoryEntry), off) != sizeof(ExtendedFileSystemDirectoryEntry)) return Array<FileInfo>();
            if (dir.size < sizeof(ExtendedFileSystemDirectoryEntry)) return Array<FileInfo>();
            if (off + dir.size > size) return Array<FileInfo>();
            if (!dir.inode) {
                off += dir.size;
                continue;
            }
            const uint16_t nameLength = dir.GetNameLength(superblock.majorVersion >= 1 && superblock.directoriesWithTypes);
            char buff[nameLength + 1] = { '\0', };
            if (ReadInode(inode, buff, nameLength, off + sizeof(ExtendedFileSystemDirectoryEntry)) != nameLength) return Array<FileInfo>();
            off += dir.size;
            if ((nameLength == 1 && buff[0] == '.') || (nameLength == 2 && buff[0] == '.' && buff[1] == '.')) continue;
            FileInfo::Type type = FileInfo::Type::Unknown;
            if (superblock.majorVersion >= 1 && superblock.directoriesWithTypes) switch (dir.typeIndicator) {
                case ExtendedFileSystemDirectoryEntry::TypeIndicator::File: {
                    type = FileInfo::Type::File;
                    break;
                }
                case ExtendedFileSystemDirectoryEntry::TypeIndicator::Directory: {
                    type = FileInfo::Type::Directory;
                    break;
                }
                default: break;
            }
            else switch ((ExtendedFileSystemInode::Type)GetInodeStruct(dir.inode).type) {
                case ExtendedFileSystemInode::Type::File: {
                    type = FileInfo::Type::File;
                    break;
                }
                case ExtendedFileSystemInode::Type::Directory: {
                    type = FileInfo::Type::Directory;
                    break;
                }
                default: break;
            }
            if (!ret.Add(FileInfo(type, String(buff)))) return Array<FileInfo>();
        }
        return ret;
    }
    bool ExtendedFileSystem::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        // TODO:
        (void)path;
        (void)overwrite;
        return false;
    }
}