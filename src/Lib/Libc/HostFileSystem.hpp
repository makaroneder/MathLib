#ifndef Freestanding
#ifndef HostFileSystem_H
#define HostFileSystem_H
#include "../FileSystem.hpp"

/// @brief File system implementation using libc file system
struct HostFileSystem : FileSystem {
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    virtual size_t OpenInternal(const String& path, const OpenMode& mode) override;
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    virtual bool Close(const size_t& file) override;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Read(const size_t& file, void* buffer, const size_t& size) override;
    /// @brief Writes data from the buffer
    /// @param file File index
    /// @param buffer Buffer to write data from
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Write(const size_t& file, const void* buffer, const size_t& size) override;
    /// @brief Sets file position
    /// @param file File index
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(const size_t& file, const ssize_t& offset, const SeekMode& mode) override;
    /// @brief Returns current file position
    /// @param file File index
    /// @return Current file position
    virtual size_t Tell(const size_t& file) override;
    /// @brief Returns file size
    /// @param file File index
    /// @return File size
    virtual size_t GetSize(const size_t& file) override;
    /// @brief Returns libc file from index
    /// @param file File index
    /// @return Libc file
    FILE* GetFile(const size_t& file);

    private:
    /// @brief List of files
    Array<FILE*> files;
};

#endif
#endif