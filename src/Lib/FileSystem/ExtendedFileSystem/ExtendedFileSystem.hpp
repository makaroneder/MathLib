#ifndef MathLib_FileSystem_ExtendedFileSystem_H
#define MathLib_FileSystem_ExtendedFileSystem_H
#include "../PhysicalFileSystem.hpp"
#include "ExtendedFileSystemInode.hpp"
#include "ExtendedFileSystemSuperblock.hpp"

namespace MathLib {
    struct ExtendedFileSystem : PhysicalFileSystem {
        ExtendedFileSystem(ByteDevice& disk);
        [[nodiscard]] virtual bool IsValid(void) const override;
        [[nodiscard]] virtual bool Create(void) override;
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) override;
        [[nodiscard]] virtual bool Close(size_t file) override;
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t GetSize(size_t file) override;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) override;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) override;

        private:
        ExtendedFileSystemSuperblock superblock;
        Array<size_t> files;
        uint32_t blockGroupCount;

        [[nodiscard]] size_t GetInode(const Sequence<char>& path);
        [[nodiscard]] ExtendedFileSystemInode GetInodeStruct(size_t inode);
        [[nodiscard]] size_t ReadInode(const ExtendedFileSystemInode& inode, void* buffer, size_t size, size_t position);
        [[nodiscard]] uint32_t GetBlockFromInode(const ExtendedFileSystemInode& inode, size_t block);
        [[nodiscard]] size_t BlockToLBA(size_t block) const;
        [[nodiscard]] size_t GetInodeBlockGroup(size_t inode) const;
        [[nodiscard]] size_t GetInodeIndex(size_t inode) const;
    };
}

#endif