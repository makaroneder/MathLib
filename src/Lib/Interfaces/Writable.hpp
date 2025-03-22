#ifndef MathLib_Interfaces_Writable_H
#define MathLib_Interfaces_Writable_H
#include "../Expected.hpp"

namespace MathLib {
    struct Writable : Allocatable {
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @return Size of written bytes
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) = 0;
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @return Status
        [[nodiscard]] bool WriteBuffer(const void* buffer, size_t size);
        /// @brief Writes text
        /// @param str Text to write
        /// @return Status
        [[nodiscard]] bool Puts(const Sequence<char>& str);
        /// @brief Writes text
        /// @param str Text to write
        /// @return Status
        [[nodiscard]] bool Puts(const char* str);
        /// @brief Writes one T value
        /// @tparam T Type of value
        /// @param value Value to write
        /// @return Status
        template <typename T>
        [[nodiscard]] bool Write(const T& value) {
            StartAndReturnFromBenchmark(WriteBuffer(&value, sizeof(T)));
        }
    };
}

#endif