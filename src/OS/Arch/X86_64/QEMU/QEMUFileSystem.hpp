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
    [[nodiscard]] bool IsValid(void) const;
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    [[nodiscard]] virtual size_t OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) override;
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    [[nodiscard]] virtual bool Close(size_t file) override;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Size of bytes read
    [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
    /// @brief Writes data to the buffer
    /// @param file File index
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Size of written bytes
    [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
    /// @brief Returns file size
    /// @param file File index
    /// @return File size
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