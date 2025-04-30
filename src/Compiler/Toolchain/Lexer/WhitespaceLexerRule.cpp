#include "WhitespaceLexerRule.hpp"
#include <Utils.hpp>

WhitespaceLexerRule::WhitespaceLexerRule(size_t type) : LexerRule(type) {}
void WhitespaceLexerRule::Match(const MathLib::Sequence<char>& str, size_t& i) const {
    const size_t size = str.GetSize();
    while (i < size && MathLib::IsWhiteSpace(str.At(i))) i++;
}