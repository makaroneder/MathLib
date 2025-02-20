#include "Writable.hpp"

namespace MathLib {
    bool Writable::WriteBuffer(const void* buffer, size_t size) {
        StartAndReturnFromBenchmark(WriteSizedBuffer(buffer, size) == size);
    }
    bool Writable::Puts(const String& str) {
        StartAndReturnFromBenchmark(WriteBuffer(str.GetValue(), str.GetSize()));
    }
    bool Writable::Puts(const char* str) {
        StartBenchmark
        size_t size = 0;
        while (str[size++]) {}
        ReturnFromBenchmark(WriteBuffer(str, size - 1));
    }
}