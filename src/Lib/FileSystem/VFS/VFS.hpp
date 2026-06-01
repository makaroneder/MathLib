#ifndef MathLib_FileSystem_VFS_H
#define MathLib_FileSystem_VFS_H
#include "VFSFile.hpp"
#include "VFSEntry.hpp"
#include "../../Pair.hpp"

namespace MathLib {
    struct VFS : FileSystem {
        VFS(void);
        [[nodiscard]] bool AddFileSystem(const VFSEntry& entry);
        [[nodiscard]] virtual bool IsValid(void) const override;
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) override;
        [[nodiscard]] virtual bool Close(size_t file) override;
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t GetSize(size_t file) override;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) override;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) override;

        private:
        [[nodiscard]] bool IsValid(size_t file) const;
        [[nodiscard]] Pair<String, size_t> GetMountpoint(const Sequence<char>& path) const;

        Array<VFSEntry> entries;
        Array<VFSFile> files;
    };
}

#endif