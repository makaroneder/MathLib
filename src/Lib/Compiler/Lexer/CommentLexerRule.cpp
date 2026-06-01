#include "CommentLexerRule.hpp"

namespace MathLib {
    CommentLexerRule::CommentLexerRule(size_t type) : LexerRule(type) {}
    void CommentLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        if (i + 1 >= size) return;
        if (str.AtUnsafe(i) != '/') return;
        if (str.AtUnsafe(i + 1) != '/') return;
        i += 2;
        while (i < size && str.AtUnsafe(i++) != '\n') {}
    }
}