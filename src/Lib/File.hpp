#ifndef File_H
#define File_H
#include "FileSystem.hpp"
#include "ByteDevice.hpp"

struct File : ByteDevice {
    FileSystem& fileSystem;
    size_t index;

    File(FileSystem& fileSystem, size_t index);
    virtual ~File(void) override;
    /// @brief Reads data to the buffer
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @return Status
    virtual bool ReadBuffer(void* buffer, const size_t& size) override;
    /// @brief Writes data from the buffer
    /// @param buffer Buffer to write data from
    /// @param size Size of the buffer
    /// @return Status
    virtual bool WriteBuffer(const void* buffer, const size_t& size) override;
    /// @brief Sets file position
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(const ssize_t& offset, const SeekMode& mode) override;
    /// @brief Returns current file position
    /// @return Current file position
    virtual size_t Tell(void) const override;
    /// @brief Returns file size
    /// @return File size
    virtual size_t GetSize(void) const override;
};

#endif