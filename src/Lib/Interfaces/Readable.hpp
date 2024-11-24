#ifndef Interfaces_Readable_H
#define Interfaces_Readable_H
#include "../Expected.hpp"

namespace MathLib {
    struct Readable : Allocatable {
        /// @brief Reads data from the buffer
        /// @param buffer Buffer to read data from
        /// @param size Size of the buffer
        /// @return Size of bytes read
        virtual size_t ReadSizedBuffer(void* buffer, size_t size) = 0;
        /// @brief Reads data to the buffer
        /// @param buffer Buffer to read data into
        /// @param size Size of the buffer
        /// @return Status
        bool ReadBuffer(void* buffer, size_t size);
        /// @brief Reads one character
        /// @return Character
        Expected<char> Getc(void);
        /// @brief Reads until certain character is found
        /// @param end Last character to read
        /// @return Read text
        String ReadUntil(char end);
        /// @brief Reads one T value
        /// @tparam T Type of value
        /// @param value Value to read
        /// @return Status
        template <typename T>
        bool Read(T& value) {
            return ReadBuffer(&value, sizeof(T));
        }
        /// @brief Reads one T value
        /// @tparam T Type of value
        /// @return Value and status
        template <typename T>
        Expected<T> Read(void) {
            T ret;
            return Read<T>(ret) ? Expected<T>(ret) : Expected<T>();
        }
    };
}

#endif