#ifndef MathLib_Database_H
#define MathLib_Database_H
#include "Typedefs.hpp"

namespace MathLib {
    struct Database : Allocatable {
        [[nodiscard]] virtual bool CreateRecord(const Sequence<char>& path, const Sequence<uint8_t>& data) = 0;
        [[nodiscard]] virtual Array<uint8_t> Read(const Sequence<char>& path) = 0;
        [[nodiscard]] virtual bool Overwrite(const Sequence<char>& path, const void* buffer, size_t size, size_t position) = 0;
        [[nodiscard]] bool DefaultOverwrite(const Sequence<char>& path, const void* buffer, size_t size, size_t position);
    };
}

#endif