#include "HexDigitLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    HexDigitLexerRule::HexDigitLexerRule(size_t type) : LexerRule(type) {}
    void HexDigitLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        if (i + 1 >= size) return;
        if (str.AtUnsafe(i) != '0') return;
        if (str.AtUnsafe(i + 1) != 'x') return;
        i += 2;
        while (i < size) {
            const char tmp = str.AtUnsafe(i);
            if (!IsDigit(tmp) && !IsBetween(ToLower(tmp), 'a', 'f')) break;
            i++;
        }
    }
}