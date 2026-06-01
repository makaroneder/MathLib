#ifndef Git_H
#define Git_H
#include "GitBlob.hpp"
#include <FileSystem/FileSystem.hpp>

struct Git : MathLib::FileSystem {
    [[nodiscard]] static MathLib::String HashToPath(const MathLib::String& hash);
    [[nodiscard]] static MathLib::String GetDefaultReference(MathLib::FileSystem& fs);
    [[nodiscard]] static MathLib::String ReferenceToCommit(MathLib::FileSystem& fs, const MathLib::String& ref);

    Git(MathLib::FileSystem& fs, const MathLib::String& rootHash);
    [[nodiscard]] virtual bool IsValid(void) const override;
    [[nodiscard]] virtual size_t OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) override;
    [[nodiscard]] virtual bool Close(size_t file) override;
    [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t GetSize(size_t file) override;
    [[nodiscard]] virtual MathLib::Array<MathLib::FileInfo> ReadDirectory(const MathLib::Sequence<char>& path) override;
    [[nodiscard]] virtual bool CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) override;
    [[nodiscard]] MathLib::String CreateBlob(const MathLib::Array<uint8_t>& data);
    [[nodiscard]] MathLib::String GetRootHash(void);

    private:
    MathLib::String rootHash;
    MathLib::Array<MathLib::String> files;
    MathLib::FileSystem& fs;
};

#endif