#include "DigitLexerRule.hpp"
#include <Utils.hpp>

DigitLexerRule::DigitLexerRule(size_t type) : LexerRule(type) {}
void DigitLexerRule::Match(const MathLib::Sequence<char>& str, size_t& i) const {
    const size_t size = str.GetSize();
    while (i < size && MathLib::IsDigit(str.At(i))) i++;
}