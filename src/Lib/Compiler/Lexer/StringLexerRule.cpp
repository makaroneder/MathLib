#include "StringLexerRule.hpp"

namespace MathLib {
    StringLexerRule::StringLexerRule(size_t type, const Sequence<char>& value) : LexerRule(type), value(MakeArray<String>(CollectionToString(value))) {}
    StringLexerRule::StringLexerRule(size_t type, const Sequence<String>& value) : LexerRule(type), value(CollectionToArray<String>(value)) {}
    void StringLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        for (const Sequence<char>& pattern : value) {
            const size_t size = pattern.GetSize();
            if (str.GetSize() < i + size) continue;
            bool match = true;
            for (size_t j = 0; j < size; j++) {
                if (str.At(i + j) != pattern.At(j)) {
                    match = false;
                    break;
                }
            }
            if (match) i += size;
        }
    }
}