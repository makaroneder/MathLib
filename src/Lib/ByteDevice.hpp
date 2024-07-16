#ifndef ByteDevice_H
#define ByteDevice_H
#include "SeekMode.hpp"
#include "Typedefs.hpp"
#include "Allocatable.hpp"

struct ByteDevice : Allocatable {
    /// @brief Reads data to the buffer
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @return Status
    virtual bool ReadBuffer(void* buffer, const size_t& size) = 0;
    /// @brief Writes data from the buffer
    /// @param buffer Buffer to write data from
    /// @param size Size of the buffer
    /// @return Status
    virtual bool WriteBuffer(const void* buffer, const size_t& size) = 0;
    /// @brief Sets position
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    virtual bool Seek(const ssize_t& offset, const SeekMode& mode) = 0;
    /// @brief Returns current position
    /// @return Current position
    virtual size_t Tell(void) const = 0;
    /// @brief Returns size
    /// @return Size
    virtual size_t GetSize(void) const = 0;
    /// @brief Reads until certain character is found
    /// @param end Last character to read
    /// @return Read text
    String ReadUntil(const char& end);
    /// @brief Writes text
    /// @param str Text to write
    /// @return Status
    bool Puts(const String& str);
    /// @brief Reads one character
    /// @return Character
    char Getc(void);
    /// @brief Reads one T value
    /// @tparam T Type of value
    /// @param value Value to read
    /// @return Status
    template <typename T>
    bool Read(T& value) {
        return ReadBuffer(&value, sizeof(T));
    }
    /// @brief Writes one T value
    /// @tparam T Type of value
    /// @param value Value to write
    /// @return Status
    template <typename T>
    bool Write(const T& value) {
        return WriteBuffer(&value, sizeof(T));
    }
};

#endif