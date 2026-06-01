#include "ExternString.hpp"

namespace MathLib {
    ExternString::ExternString(char* buffer) : buffer(buffer) {}
    bool ExternString::Add(const char&) {
        return false;
    }
    size_t ExternString::GetSize(void) const {
        if (!buffer) return 0;
        size_t i = 0;
        while (buffer[i]) i++;
        return i;
    }
    char* ExternString::GetValue(void) {
        return buffer;
    }
    const char* ExternString::GetValue(void) const {
        return buffer;
    }
    bool ExternString::Reset(void) {
        return false;
    }
}