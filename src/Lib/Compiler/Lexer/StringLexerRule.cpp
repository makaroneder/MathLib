#include "StringLexerRule.hpp"

namespace MathLib {
    StringLexerRule::StringLexerRule(size_t type) : LexerRule(type) {}
    void StringLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        if (i >= size) return;
        if (str.AtUnsafe(i) != '"') return;
        i++;
        while (i < size && str.AtUnsafe(i++) != '"') {}
    }
}