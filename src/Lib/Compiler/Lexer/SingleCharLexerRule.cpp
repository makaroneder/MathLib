#include "SingleCharLexerRule.hpp"

namespace MathLib {
    SingleCharLexerRule::SingleCharLexerRule(size_t type, const Sequence<char>& chars) : LexerRule(type), chars(CollectionToArray<char>(chars)) {}
    void SingleCharLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        i += i < str.GetSize() && chars.Contains(str.AtUnsafe(i));
    }
}