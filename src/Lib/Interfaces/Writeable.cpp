#include "Writeable.hpp"

namespace MathLib {
    bool Writeable::WriteBuffer(const void* buffer, size_t size) {
        StartBenchmark
        ReturnFromBenchmark(WriteSizedBuffer(buffer, size) == size);
    }
    bool Writeable::Puts(const String& str) {
        StartBenchmark
        ReturnFromBenchmark(WriteBuffer(str.GetValue(), str.GetSize()));
    }
    bool Writeable::Puts(const char* str) {
        StartBenchmark
        size_t size = 0;
        while (str[size++]) {}
        ReturnFromBenchmark(WriteBuffer(str, size - 1));
    }
}