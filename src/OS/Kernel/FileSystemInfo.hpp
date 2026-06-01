#ifndef FileSystemInfo_H
#define FileSystemInfo_H
#include <FileSystem/FileSystem.hpp>

struct FileSystemInfo;
struct FileSystemInfo : MathLib::FileSystem, MathLib::Comparable<FileSystemInfo> {
    MathLib::String name;

    FileSystemInfo(void);
    FileSystemInfo(MathLib::FileSystem* fs, const MathLib::String& name);
    [[nodiscard]] MathLib::FileSystem* ReleaseFileSystem(void);
    [[nodiscard]] virtual bool IsValid(void) const override;
    [[nodiscard]] virtual size_t OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) override;
    [[nodiscard]] virtual bool Close(size_t file) override;
    [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t GetSize(size_t file) override;
    [[nodiscard]] virtual MathLib::Array<MathLib::FileInfo> ReadDirectory(const MathLib::Sequence<char>& path) override;
    [[nodiscard]] virtual bool CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) override;

    protected:
    [[nodiscard]] virtual bool Equals(const FileSystemInfo& other) const override;

    private:
    MathLib::FileSystem* fs;
};

#endif