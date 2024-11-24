#ifndef CharBuffer_H
#define CharBuffer_H
#include "Buffer.hpp"
#include "Memory.hpp"

namespace MathLib {
    struct CharBuffer : Buffer<char> {
        CharBuffer(void) : CharBuffer("") {}
        CharBuffer(const char* str) : Buffer<char>() {
            for (; str[size]; size++) {}
            buffer = new char[size + 1];
            MemoryCopy(str, buffer, size);
            buffer[size] = '\0';
        }
        CharBuffer(const char* str, size_t size) : Buffer<char>() {
            buffer = new char[size + 1];
            MemoryCopy(str, buffer, size);
            buffer[size] = '\0';
        }
        CharBuffer(const char& chr) {
            buffer = new char[2];
            buffer[0] = chr;
            buffer[1] = '\0';
            size = 1;
        }
        CharBuffer& operator+=(const CharBuffer& other) {
            char* ptr = new char[size + other.size + 1];
            if (!ptr) return *this;
            MemoryCopy(buffer, ptr, size);
            MemoryCopy(other.buffer, ptr + size, other.size);
            size += other.size;
            ptr[size] = '\0';
            delete [] buffer;
            buffer = ptr;
            return *this;
        }
        CharBuffer& operator+=(const char* other) {
            return *this += CharBuffer(other);
        }
        CharBuffer& operator+=(char chr) {
            char* ptr = new char[size + 2];
            if (!ptr) return *this;
            MemoryCopy(buffer, ptr, size);
            ptr[size++] = chr;
            ptr[size] = '\0';
            delete [] buffer;
            buffer = ptr;
            return *this;
        }
        CharBuffer operator+(const CharBuffer& other) const {
            CharBuffer tmp = *this;
            tmp += other;
            return tmp;
        }
        CharBuffer operator+(const char* other) const {
            CharBuffer tmp = *this;
            tmp += other;
            return tmp;
        }
        CharBuffer operator+(char chr) const {
            CharBuffer tmp = *this;
            tmp += chr;
            return tmp;
        }
        bool operator==(const CharBuffer& other) const {
            if (size != other.size) return false;
            for (size_t i = 0; i < size; i++)
                if (At(i) != other.At(i)) return false;
            return true;
        }
        bool operator==(const char* other) const {
            return *this == CharBuffer(other);
        }
        bool operator!=(const CharBuffer& other) const {
            return !(*this == other);
        }
        bool operator!=(const char* other) const {
            return !(*this == other);
        }
    };
}

#endif