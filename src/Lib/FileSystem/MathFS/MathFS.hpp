#ifndef MathLib_FileSystem_MathFS_H
#define MathLib_FileSystem_MathFS_H
#include "../PhysicalFileSystem.hpp"
#include "MathFSFile.hpp"

namespace MathLib {
    struct MathFS : PhysicalFileSystem {
        MathFS(ByteDevice& disk);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] virtual bool Create(void) override;
        /// @brief Opens file
        /// @param path Path to file
        /// @param mode Mode to open file in
        /// @return File index
        [[nodiscard]] virtual size_t OpenInternal(const String& path, OpenMode mode) override;
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
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const String& path) override;

        private:
        uint64_t root;
        Array<MathFSFile> files;
    };
}

#endif