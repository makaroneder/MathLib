#ifndef MathLib_FileSystem_H
#define MathLib_FileSystem_H
#include "SeekMode.hpp"
#include "OpenMode.hpp"
#include "FileInfo.hpp"
#include "File.hpp"

namespace MathLib {
    /// @brief File system
    struct FileSystem : Allocatable {
        /// @brief Opens file
        /// @param path Path to file
        /// @param mode Mode to open file in
        /// @return File
        [[nodiscard]] File Open(const Sequence<char>& path, OpenMode mode);
        /// @brief Opens file
        /// @param path Path to file
        /// @param mode Mode to open file in
        /// @return File index
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) = 0;
        /// @brief Closes file
        /// @param file File index
        /// @return Status
        [[nodiscard]] virtual bool Close(size_t file) = 0;
        /// @brief Reads data to the buffer
        /// @param file File index
        /// @param buffer Buffer to read data into
        /// @param size Size of the buffer
        /// @param position Position to read from
        /// @return Size of bytes read
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) = 0;
        /// @brief Writes data to the buffer
        /// @param file File index
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @param position Position to write to
        /// @return Size of written bytes
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) = 0;
        /// @brief Returns file size
        /// @param file File index
        /// @return File size
        [[nodiscard]] virtual size_t GetSize(size_t file) = 0;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) = 0;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) = 0;
        [[nodiscard]] String ListFiles(const Sequence<char>& path, size_t maxDepth, const Sequence<char>& padding = ""_M);
    };
}

#endif