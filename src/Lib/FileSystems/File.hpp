#ifndef File_H
#define File_H
#include "FileSystem.hpp"
#include "../Interfaces/ByteDevice.hpp"

struct File : ByteDevice {
    FileSystem& fileSystem;
    size_t index;

    File(FileSystem& fileSystem, size_t index);
    virtual ~File(void) override;
    /// @brief Returns file size
    /// @return File size
    virtual size_t GetSize(void) const override;
    /// @brief Reads data from the buffer
    /// @param buffer Buffer to read data from
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Size of bytes read
    virtual size_t ReadPositionedSizedBuffer(void* buffer, const size_t& size, const size_t& position) override;
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Size of written bytes
    virtual size_t WritePositionedSizedBuffer(const void* buffer, const size_t& size, const size_t& position) override;
};

#endif