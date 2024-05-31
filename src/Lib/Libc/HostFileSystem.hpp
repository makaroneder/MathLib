#ifndef Freestanding
#ifndef HostFileSystem_H
#define HostFileSystem_H
#include "../FileSystem.hpp"
#include "../Typedefs.hpp"

/// @brief File system implementation using libc file system
struct HostFileSystem : FileSystem {
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    virtual size_t Open(String path, OpenMode mode) override;
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    virtual bool Close(size_t file) override;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Read(size_t file, void* buffer, size_t size) override;
    /// @brief Writes data from the buffer
    /// @param file File index
    /// @param buffer Buffer to write data from
    /// @param size Size of the buffer
    /// @return Status
    virtual bool Write(size_t file, const void* buffer, size_t size) override;
    /// @brief Updates file
    /// @param file File index
    /// @return Status
    virtual bool Flush(size_t file) override;
    /// @brief Sets file position
    /// @param file File index
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(size_t file, ssize_t offset, SeekMode mode) override;
    /// @brief Returns current file position
    /// @param file File index
    /// @return Current file position
    virtual size_t Tell(size_t file) override;
    /// @brief Returns libc file from index
    /// @param file File index
    /// @return Libc file
    FILE* GetFile(size_t file);

    private:
    /// @brief List of files
    Array<FILE*> files;
};

#endif
#endif