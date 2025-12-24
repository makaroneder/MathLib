#ifndef Git_H
#define Git_H
#include "GitBlob.hpp"
#include <FileSystem/FileSystem.hpp>

struct Git : MathLib::FileSystem {
    Git(MathLib::FileSystem& fs, const MathLib::String& rootHash);
    [[nodiscard]] virtual size_t OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) override;
    [[nodiscard]] virtual bool Close(size_t file) override;
    [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t GetSize(size_t file) override;
    [[nodiscard]] virtual MathLib::Array<MathLib::FileInfo> ReadDirectory(const MathLib::Sequence<char>& path) override;
    [[nodiscard]] virtual bool CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) override;
    MathLib::String CreateBlob(const MathLib::Array<uint8_t>& data);

    private:
    MathLib::String rootHash;
    MathLib::Array<MathLib::String> files;
    MathLib::FileSystem& fs;
};

#endif