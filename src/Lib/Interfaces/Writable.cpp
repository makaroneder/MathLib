#include "Writable.hpp"

namespace MathLib {
    bool Writable::WriteBuffer(const void* buffer, size_t size) {
        return WriteSizedBuffer(buffer, size) == size;
    }
    bool Writable::Puts(const Sequence<char>& str) {
        const size_t size = str.GetSize();
        char buff[size];
        for (size_t i = 0; i < size; i++) buff[i] = str.At(i);
        return WriteBuffer(buff, size);
    }
    bool Writable::Puts(const char* str) {
        size_t size = 0;
        while (str[size]) size++;
        return WriteBuffer(str, size);
    }
}