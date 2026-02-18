#ifndef Freestanding
#include "HostLogger.hpp"

namespace MathLib {
    HostLogger::HostLogger(std::ostream& stream) : stream(stream) {}
    size_t HostLogger::WriteSizedBuffer(const void* buffer, size_t size) {
        stream.write((const char*)buffer, size);
        return size;
    }
}

#endif