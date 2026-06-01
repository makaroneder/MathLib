#ifdef __x86_64__
#ifndef QEMUFileSystem_H
#define QEMUFileSystem_H
#include "QEMUFileEntry.hpp"
#include "QEMUFile.hpp"
#include <FileSystem/FileSystem.hpp>

struct QEMUFileSystem : MathLib::FileSystem {
    static constexpr const char* expectedSignature = "QEMU";
    enum class IOOffset : uint16_t {
        Selector = 0x0510,
        Data,
        DMA = 0x0514,
    };
    [[nodiscard]] virtual bool IsValid(void) const override;
    [[nodiscard]] virtual size_t OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) override;
    [[nodiscard]] virtual bool Close(size_t file) override;
    [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
    [[nodiscard]] virtual size_t GetSize(size_t file) override;
    [[nodiscard]] virtual MathLib::Array<MathLib::FileInfo> ReadDirectory(const MathLib::Sequence<char>& path) override;
    [[nodiscard]] virtual bool CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) override;

    private:
    bool Foreach(const MathLib::Function<bool, QEMUFileEntry>& function);
    void SetSelector(QEMUSelector selector) const;
    void Read(void* buffer, size_t size) const;

    MathLib::Array<QEMUFile> files;
};

#endif
#endif