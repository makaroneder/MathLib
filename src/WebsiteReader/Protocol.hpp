#ifndef Protocol_H
#define Protocol_H
#include <Interfaces/RWDevice.hpp>

struct Protocol : MathLib::Allocatable {
    [[nodiscard]] virtual MathLib::String GetScheme(void) const = 0;
    [[nodiscard]] virtual size_t GetDefaultPort(void) const = 0;
    [[nodiscard]] virtual MathLib::String Read(MathLib::RWDevice& socket, const MathLib::String& host, const MathLib::String& path) const = 0;
};

#endif