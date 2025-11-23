#include "FloatLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    FloatLexerRule::FloatLexerRule(size_t type) : LexerRule(type) {}
    void FloatLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        while (i < size && IsDigit(str.At(i))) i++;
        if (i < size && str.At(i) == '.') {
            i++;
            while (i < size && IsDigit(str.At(i))) i++;
        }
    }
}