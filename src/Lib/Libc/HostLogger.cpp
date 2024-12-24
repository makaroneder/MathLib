#ifndef Freestanding
#include "HostLogger.hpp"

namespace MathLib {
    HostLogger::HostLogger(std::ostream& stream) : stream(stream) {
        EmptyBenchmark
    }
    size_t HostLogger::WriteSizedBuffer(const void* buffer, size_t size) {
        StartBenchmark
        stream.write((const char*)buffer, size);
        ReturnFromBenchmark(size);
    }
}

#endif