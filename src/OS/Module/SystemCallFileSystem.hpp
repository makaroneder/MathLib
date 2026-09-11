#ifndef Module_SystemCallFileSystem_H
#define Module_SystemCallFileSystem_H
#include <FileSystem/FileSystem.hpp>

struct SystemCallFileSystem : MathLib::FileSystem {
    [[nodiscard]] virtual bool IsValid(void) const override;
    [[nodiscard]] virtual size_t OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) override;
    [[nodiscard]] virtual bool Close(size_t file) override;
    [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t GetSize(size_t file) override;
    [[nodiscard]] virtual MathLib::Array<MathLib::FileInfo> ReadDirectory(const MathLib::Sequence<char>& path) override;
    [[nodiscard]] virtual bool CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) override;
};

#endif