#include "Typedefs.hpp"

namespace MathLib {
    num_t eps = 1e-3;

    String CollectionToString(const Sequence<char>& sequence) {
        const size_t size = sequence.GetSize();
        String ret;
        for (size_t i = 0; i < size; i++) ret += sequence.AtUnsafe(i);
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