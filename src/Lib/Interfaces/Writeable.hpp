#ifndef MathLib_Interfaces_Writeable_H
#define MathLib_Interfaces_Writeable_H
#include "../Expected.hpp"

namespace MathLib {
    struct Writeable : Allocatable {
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @return Size of written bytes
        virtual size_t WriteSizedBuffer(const void* buffer, size_t size) = 0;
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @return Status
        bool WriteBuffer(const void* buffer, size_t size);
        /// @brief Writes text
        /// @param str Text to write
        /// @return Status
        bool Puts(const String& str);
        /// @brief Writes text
        /// @param str Text to write
        /// @return Status
        bool Puts(const char* str);
        /// @brief Writes one T value
        /// @tparam T Type of value
        /// @param value Value to write
        /// @return Status
        template <typename T>
        bool Write(const T& value) {
            StartBenchmark
            ReturnFromBenchmark(WriteBuffer(&value, sizeof(T)));
        }
    };
}

#endif