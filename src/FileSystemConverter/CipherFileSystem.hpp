#ifndef CipherFileSystem_H
#define CipherFileSystem_H
#include "FileCipher.hpp"
#include "CipherFile.hpp"
#include <FileSystem/PhysicalFileSystem.hpp>
#include <Interfaces/ByteArray.hpp>

struct CipherFileSystem : MathLib::PhysicalFileSystem {
    CipherFileSystem(MathLib::ByteDevice& disk, FileCipher* cipher, const MathLib::Sequence<uint64_t>& key);
    virtual ~CipherFileSystem(void) override;
    [[nodiscard]] virtual bool IsValid(void) const override;
    [[nodiscard]] virtual bool Create(void) override;
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

    private:
    [[nodiscard]] size_t WriteInternal(size_t& newSize, MathLib::ByteArray& data, const MathLib::Sequence<char>& path, size_t prevSize, size_t writePosition, const void* buffer, size_t size, size_t position);

    FileCipher* cipher;
    MathLib::Array<uint64_t> key;
    MathLib::Array<CipherFile> files;
};

#endif