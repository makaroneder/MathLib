#include "Readable.hpp"

namespace MathLib {
    bool Readable::ReadBuffer(void* buffer, size_t size) {
        StartAndReturnFromBenchmark(ReadSizedBuffer(buffer, size) == size);
    }
    Expected<char> Readable::Getc(void) {
        StartBenchmark
        char chr;
        ReturnFromBenchmark(Read<char>(chr) ? Expected<char>(chr) : Expected<char>());
    }
    String Readable::ReadUntil(char end) {
        StartBenchmark
        String ret;
        while (true) {
            const Expected<char> chr = Getc();
            if (!chr.HasValue() || chr.Get() == end) break;
            ret += chr.Get();
        }
        ReturnFromBenchmark(ret);
    }
    bool Readable::DefaultSkip(size_t size) {
        StartBenchmark
        uint8_t buffer[size];
        ReturnFromBenchmark(ReadBuffer(buffer, size));
    }
}