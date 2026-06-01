#include "Typedefs.hpp"

namespace MathLib {
    num_t eps = 1e-3;

    String CollectionToString(const Sequence<char>& sequence) {
        const size_t size = sequence.GetSize();
        String ret;
        for (size_t i = 0; i < size; i++) ret += sequence.AtUnsafe(i);
        return ret;
    }
    uint8_t StringToU4(char chr) {
        switch (chr) {
            case '0' ... '9': return chr - '0';
            case 'a' ... 'f': return chr - 'a' + 0xa;
            case 'A' ... 'F': return chr - 'A' + 0xa;
            default: return UINT8_MAX;
        }
    }
    uint8_t StringToU8(char a, char b) {
        return b ? (StringToU4(a) << 4) | StringToU4(b) : StringToU4(a);
    }
    uint64_t StringToU64(const Sequence<char>& str) {
        const size_t size = str.GetSize();
        uint64_t ret = 0;
        for (size_t i = 0; i < size; i++)
            ret = (ret << 4) | StringToU4(str.AtUnsafe(i));
        return ret;
    }
}
MathLib::String operator""_M(const char* str, size_t size) {
    (void)size;
    return MathLib::String(str);
}
MathLib::String operator""_M(char chr) {
    return MathLib::String(chr);
}