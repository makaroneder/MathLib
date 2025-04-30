#include "IdentifierLexerRule.hpp"
#include <Utils.hpp>

IdentifierLexerRule::IdentifierLexerRule(size_t type) : LexerRule(type) {}
void IdentifierLexerRule::Match(const MathLib::Sequence<char>& str, size_t& i) const {
    const size_t size = str.GetSize();
    if (i >= size) return;
    char chr = str.At(i);
    if (chr != '_' && !MathLib::IsAlpha(chr)) return;
    while (++i < size) {
        chr = str.At(i);
        if (chr != '_' && !MathLib::IsAlphaDigit(chr)) return;
    }
}