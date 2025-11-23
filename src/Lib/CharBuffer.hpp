#ifndef MathLib_CharBuffer_H
#define MathLib_CharBuffer_H
#include "Buffer.hpp"
#include "Memory.hpp"

namespace MathLib {
    struct CharBuffer : Buffer<char> {
        using Sequence<char>::operator==;
        using Sequence<char>::operator!=;
        using Collection<char>::operator+=;
        CharBuffer(void) : CharBuffer("") {}
        CharBuffer(const char* str) {
            for (; str[size]; size++) {}
            buffer = new char[size + 1];
            MemoryCopy(str, buffer, size);
            buffer[size] = '\0';
        }
        CharBuffer(const char* str, size_t size) {
            buffer = new char[size + 1];
            MemoryCopy(str, buffer, size);
            buffer[size] = '\0';
            this->size = size;
        }
        CharBuffer(char chr) {
            buffer = new char[2];
            buffer[0] = chr;
            buffer[1] = '\0';
            size = 1;
        }
        CharBuffer& operator+=(const char* other) {
            size_t otherSize = 0;
            while (other[otherSize]) otherSize++;
            char* ptr = new char[size + otherSize + 1];
            if (!ptr) return *this;
            MemoryCopy(buffer, ptr, size);
            MemoryCopy(other, ptr + size, otherSize);
            size += otherSize;
            ptr[size] = '\0';
            delete [] buffer;
            buffer = ptr;
            return *this;
        }
        [[nodiscard]] CharBuffer operator+(const Sequence<char>& other) const {
            CharBuffer tmp = *this;
            tmp += other;
            return tmp;
        }
        [[nodiscard]] CharBuffer operator+(const char* other) const {
            CharBuffer tmp = *this;
            tmp += other;
            return tmp;
        }
        [[nodiscard]] CharBuffer operator+(char chr) const {
            CharBuffer tmp = *this;
            tmp += chr;
            return tmp;
        }
        [[nodiscard]] bool operator==(const char* other) const {
            return *this == CharBuffer(other);
        }
        [[nodiscard]] bool operator!=(const char* other) const {
            return *this != CharBuffer(other);
        }
    };
}

#endif