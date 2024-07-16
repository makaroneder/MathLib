#ifndef E9_H
#define E9_H
#include <ByteDevice.hpp>

struct E9 : ByteDevice {
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
    /// @brief Sets position
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(const ssize_t& offset, const SeekMode& mode) override;
    /// @brief Returns current position
    /// @return Current position
    virtual size_t Tell(void) const override;
    /// @brief Returns size
    /// @return Size
    virtual size_t GetSize(void) const override;
};

#endif