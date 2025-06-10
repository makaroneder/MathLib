#include "IdentifierLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    IdentifierLexerRule::IdentifierLexerRule(size_t type) : LexerRule(type) {
        EmptyBenchmark
    }
    void IdentifierLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        StartBenchmark
        const size_t size = str.GetSize();
        if (i >= size) ReturnFromBenchmark();
        char chr = str.At(i);
        if (chr != '_' && !IsAlpha(chr)) ReturnFromBenchmark();
        while (++i < size) {
            chr = str.At(i);
            if (chr != '_' && !IsAlphaDigit(chr)) ReturnFromBenchmark();
        }
        ReturnFromBenchmark();
    }
}