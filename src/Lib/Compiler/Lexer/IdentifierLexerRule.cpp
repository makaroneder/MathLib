#include "IdentifierLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    IdentifierLexerRule::IdentifierLexerRule(size_t type, bool startWithDigit) : LexerRule(type), startWithDigit(startWithDigit) {}
    void IdentifierLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        if (i >= size) return;
        if (!startWithDigit) {
            const char chr = str.At(i);
            if (chr != '_' && !IsAlpha(chr)) return;
            i++;
        }
        for (; i < size; i++) {
            const char chr = str.At(i);
            if (chr != '_' && !IsAlphaDigit(chr)) return;
        }
    }
}