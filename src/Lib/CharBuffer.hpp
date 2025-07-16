#ifndef MathLib_CharBuffer_H
#define MathLib_CharBuffer_H
#include "Buffer.hpp"
#include "Memory.hpp"
#include "Benchmark.hpp"

namespace MathLib {
    struct CharBuffer : Buffer<char> {
        using Sequence<char>::operator==;
        using Sequence<char>::operator!=;
        using Collection<char>::operator+=;
        CharBuffer(void) : CharBuffer("") {
            EmptyBenchmark
        }
        CharBuffer(const char* str) : Buffer<char>() {
            StartBenchmark
            for (; str[size]; size++) {}
            buffer = new char[size + 1];
            MemoryCopy(str, buffer, size);
            buffer[size] = '\0';
            EndBenchmark
        }
        CharBuffer(const char* str, size_t size) : Buffer<char>() {
            StartBenchmark
            buffer = new char[size + 1];
            MemoryCopy(str, buffer, size);
            buffer[size] = '\0';
            this->size = size;
            EndBenchmark
        }
        CharBuffer(const char& chr) {
            StartBenchmark
            buffer = new char[2];
            buffer[0] = chr;
            buffer[1] = '\0';
            size = 1;
            EndBenchmark
        }
        CharBuffer& operator+=(const char* other) {
            StartBenchmark
            size_t otherSize = 0;
            while (other[otherSize]) otherSize++;
            char* ptr = new char[size + otherSize + 1];
            if (!ptr) ReturnFromBenchmark(*this);
            MemoryCopy(buffer, ptr, size);
            MemoryCopy(other, ptr + size, otherSize);
            size += otherSize;
            ptr[size] = '\0';
            delete [] buffer;
            buffer = ptr;
            ReturnFromBenchmark(*this);
        }
        [[nodiscard]] CharBuffer operator+(const Sequence<char>& other) const {
            StartBenchmark
            CharBuffer tmp = *this;
            tmp += other;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] CharBuffer operator+(const char* other) const {
            StartBenchmark
            CharBuffer tmp = *this;
            tmp += other;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] CharBuffer operator+(char chr) const {
            StartBenchmark
            CharBuffer tmp = *this;
            tmp += chr;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] bool operator==(const char* other) const {
            StartAndReturnFromBenchmark(*this == CharBuffer(other));
        }
        [[nodiscard]] bool operator!=(const char* other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }
    };
}

#endif