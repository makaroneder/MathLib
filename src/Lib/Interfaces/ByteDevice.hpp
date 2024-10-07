#ifndef Interfaces_ByteDevice_H
#define Interfaces_ByteDevice_H
#include "../SeekMode.hpp"
#include "Readable.hpp"
#include "Writeable.hpp"

struct ByteDevice : Readable, Writeable {
    ByteDevice(void);
    /// @brief Returns size
    /// @return Size
    virtual size_t GetSize(void) const = 0;
    /// @brief Reads data from the buffer
    /// @param buffer Buffer to read data from
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Size of bytes read
    virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) = 0;
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Size of written bytes
    virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) = 0;

    /// @brief Reads data from the buffer
    /// @param buffer Buffer to read data from
    /// @param size Size of the buffer
    /// @return Size of bytes read
    virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @return Size of written bytes
    virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
    /// @brief Reads data to the buffer
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Status
    bool ReadPositionedBuffer(void* buffer, size_t size, size_t position);
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Status
    bool WritePositionedBuffer(const void* buffer, size_t size, size_t position);
    /// @brief Sets position
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    bool Seek(ssize_t offset, SeekMode mode);
    /// @brief Returns current position
    /// @return Current position
    size_t Tell(void) const;
    /// @brief Reads one T value
    /// @tparam T Type of value
    /// @param value Value to read
    /// @param position Position to read from
    /// @return Status
    template <typename T>
    bool ReadPositioned(T& value, size_t position) {
        return ReadPositionedBuffer(&value, sizeof(T), position);
    }
    /// @brief Writes one T value
    /// @tparam T Type of value
    /// @param value Value to write
    /// @param position Position to write to
    /// @return Status
    template <typename T>
    bool WritePositioned(const T& value, size_t position) {
        return WritePositionedBuffer(&value, sizeof(T), position);
    }
    /// @brief Reads one T value
    /// @tparam T Type of value
    /// @param position Position to read from
    /// @return Value and status
    template <typename T>
    Expected<T> ReadPositioned(size_t position) {
        T ret;
        return ReadPositioned<T>(ret, position) ? Expected<T>(ret) : Expected<T>();
    }

    private:
    size_t position;
};

#endif