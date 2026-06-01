#ifndef MathLib_Interfaces_Writable_H
#define MathLib_Interfaces_Writable_H
#include "../Expected.hpp"

namespace MathLib {
    struct Writable : Allocatable {
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) = 0;
        [[nodiscard]] bool WriteBuffer(const void* buffer, size_t size);
        [[nodiscard]] bool Puts(const Collection<char>& str);
        [[nodiscard]] bool Puts(const Sequence<char>& str);
        [[nodiscard]] bool Puts(const char* str);
        [[nodiscard]] bool Puts(char chr);
        template <typename T>
        [[nodiscard]] bool Write(const T& value) {
            return WriteBuffer(&value, sizeof(T));
        }
        template <typename T>
        [[nodiscard]] bool WriteCollection(const Collection<T>& sequence) {
            return WriteBuffer(sequence.GetValue(), sequence.GetSize() * sizeof(T));
        }
    };
}

#endif