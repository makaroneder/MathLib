#ifndef MathLib_FileSystem_Directory_H
#define MathLib_FileSystem_Directory_H
#include "FileSystem.hpp"

namespace MathLib {
    struct Directory : FileSystem {
        Directory(FileSystem& base, const Sequence<char>& basePath);
        [[nodiscard]] virtual bool IsValid(void) const override;
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) override;
        [[nodiscard]] virtual bool Close(size_t file) override;
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t GetSize(size_t file) override;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) override;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) override;

        private:
        String basePath;
        FileSystem& base;
    };
}

#endif