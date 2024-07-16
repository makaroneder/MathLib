#ifndef FileSystem_H
#define FileSystem_H
#include "SeekMode.hpp"
#include "Typedefs.hpp"
#include "Allocatable.hpp"

struct File;
/// @brief Mode to open file in
enum class OpenMode {
    Read,
    Write,
};
/// @brief File system
struct FileSystem : Allocatable {
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File
    File Open(const String& path, const OpenMode& mode);
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    virtual bool Close(const size_t& file) = 0;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Read(const size_t& file, void* buffer, const size_t& size) = 0;
    /// @brief Writes data from the buffer
    /// @param file File index
    /// @param buffer Buffer to write data from
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Write(const size_t& file, const void* buffer, const size_t& size) = 0;
    /// @brief Sets file position
    /// @param file File index
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(const size_t& file, const ssize_t& offset, const SeekMode& mode) = 0;
    /// @brief Returns current file position
    /// @param file File index
    /// @return Current file position
    virtual size_t Tell(const size_t& file) = 0;
    /// @brief Returns file size
    /// @param file File index
    /// @return File size
    virtual size_t GetSize(const size_t& file) = 0;

    protected:
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    virtual size_t OpenInternal(const String& path, const OpenMode& mode) = 0;
};

#endif