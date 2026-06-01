#include "ASLIdentifierLexerRule.hpp"
#include <Utils.hpp>

ASLIdentifierLexerRule::ASLIdentifierLexerRule(size_t type) : LexerRule(type) {}
void ASLIdentifierLexerRule::Match(const MathLib::Sequence<char>& str, size_t& i) const {
    const size_t size = str.GetSize();
    if (i >= size) return;
    const size_t save = i;
    bool canBeEmpty = false;
    if (str.AtUnsafe(i) == '\\') {
        canBeEmpty = true;
        i++;
    }
    else if (str.AtUnsafe(i) == '^') {
        i++;
        while (i < size && str.At(i) == '^') i++;
        canBeEmpty = i != save + 1;
    }
    bool first = true;
    if (i >= size && !canBeEmpty) i = save;
    else while (i < size) {
        const char tmp = str.AtUnsafe(i);
        const bool notValidFirst = !MathLib::IsAlpha(tmp) && tmp != '_';
        if (notValidFirst && first) {
            if (canBeEmpty) break;
            i = save;
            return;
        }
        first = canBeEmpty = false;
        if (tmp == '.') first = true;
        else if (notValidFirst && !MathLib::IsDigit(tmp)) break;
        i++;
    }
}