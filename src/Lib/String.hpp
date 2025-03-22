#ifndef MathLib_String_H
#define MathLib_String_H
#include "Typedefs.hpp"

namespace MathLib {
    [[nodiscard]] String Erase(const Sequence<char>& str, size_t pos, size_t len);
    [[nodiscard]] String SubString(const Sequence<char>& str, size_t pos, size_t len);
    [[nodiscard]] Array<String> Split(const Sequence<char>& str, const Sequence<char>& delim, bool preserveDelim);
    [[nodiscard]] String ToString(size_t x, size_t base, size_t size = 0);
    [[nodiscard]] String BoolToString(bool x);
    [[nodiscard]] String DumpMemory(uintptr_t addr, size_t size, size_t lineSize);
    [[nodiscard]] bool Match(const Sequence<char>& str, const Sequence<char>& pattern);
}

#endif