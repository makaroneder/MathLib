#ifndef MathLib_FileSystem_VFS_H
#define MathLib_FileSystem_VFS_H
#include "VFSFile.hpp"
#include "VFSEntry.hpp"

namespace MathLib {
    struct VFS : FileSystem {
        VFS(void);
        /// @brief Adds file system to VFS
        /// @param entry File system and it's name
        /// @return Status
        [[nodiscard]] bool AddFileSystem(const VFSEntry& entry);
        /// @brief Opens file
        /// @param path Path to file
        /// @param mode Mode to open file in
        /// @return File index
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) override;
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
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) override;

        private:
        [[nodiscard]] bool IsValid(size_t file) const;

        Array<VFSEntry> entries;
        Array<VFSFile> files;
    };
}

#endif