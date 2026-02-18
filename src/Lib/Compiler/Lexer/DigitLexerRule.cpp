#include "DigitLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    DigitLexerRule::DigitLexerRule(size_t type) : LexerRule(type) {}
    void DigitLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        while (i < size && IsDigit(str.At(i))) i++;
    }
}