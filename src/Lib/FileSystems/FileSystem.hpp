#ifndef FileSystem_H
#define FileSystem_H
#include "SeekMode.hpp"
#include "OpenMode.hpp"
#include "FileInfo.hpp"

struct File;
/// @brief File system
struct FileSystem : Allocatable {
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File
    File Open(const String& path, OpenMode mode);
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    virtual size_t OpenInternal(const String& path, OpenMode mode) = 0;
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    virtual bool Close(size_t file) = 0;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Size of bytes read
    virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) = 0;
    /// @brief Writes data to the buffer
    /// @param file File index
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Size of written bytes
    virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) = 0;
    /// @brief Returns file size
    /// @param file File index
    /// @return File size
    virtual size_t GetSize(size_t file) = 0;
    virtual Array<FileInfo> ReadDirectory(const String& path) = 0;
    String ListFiles(const String& path, const String& padding = "");
};

#endif