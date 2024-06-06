#ifndef FileSystem_H
#define FileSystem_H
#include "SeekMode.hpp"
#include "Typedefs.hpp"

/// @brief Mode to open file in
enum class OpenMode {
    Read,
    Write,
    ReadByte,
    WriteByte,
};
/// @brief File system
struct FileSystem {
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    virtual size_t Open(String path, OpenMode mode) = 0;
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    virtual bool Close(size_t file) = 0;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Read(size_t file, void* buffer, size_t size) = 0;
    /// @brief Writes data from the buffer
    /// @param file File index
    /// @param buffer Buffer to write data from
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Write(size_t file, const void* buffer, size_t size) = 0;
    /// @brief Updates file
    /// @param file File index
    /// @return Status
    virtual bool Flush(size_t file) = 0;
    /// @brief Sets file position
    /// @param file File index
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(size_t file, ssize_t offset, SeekMode mode) = 0;
    /// @brief Returns current file position
    /// @param file File index
    /// @return Current file position
    virtual size_t Tell(size_t file) = 0;
    /// @brief Returns file size
    /// @param file File index
    /// @return File size
    size_t GetFileSize(size_t file);
    /// @brief Reads until certain character is found
    /// @param file File index
    /// @param end Last character to read
    /// @return Read text
    String ReadUntil(size_t file, char end);
    /// @brief Writes text
    /// @param file File index
    /// @param str Text to write
    /// @return Status
    bool Puts(size_t file, String str);
    /// @brief Reads one character
    /// @param file File index
    /// @return Character
    char Getc(size_t file);
};

#endif