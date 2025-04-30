#include "SingleCharLexerRule.hpp"

SingleCharLexerRule::SingleCharLexerRule(size_t type, const MathLib::Sequence<char>& chars) : LexerRule(type), chars(MathLib::CollectionToArray<char>(chars)) {}
void SingleCharLexerRule::Match(const MathLib::Sequence<char>& str, size_t& i) const {
    i += i < str.GetSize() && chars.Contains(str.At(i));
}