#ifndef MathLib_String_H
#define MathLib_String_H
#include "Typedefs.hpp"

namespace MathLib {
    [[nodiscard]] String Erase(String str, size_t pos, size_t len);
    [[nodiscard]] String SubString(String str, size_t pos, size_t len);
    [[nodiscard]] Array<String> Split(String str, String delim, bool preserveDelim);
    [[nodiscard]] String ToString(size_t x, size_t base, size_t size = 0);
    [[nodiscard]] String BoolToString(bool x);
    [[nodiscard]] String DumpMemory(uintptr_t addr, size_t size, size_t lineSize);
}

#endif