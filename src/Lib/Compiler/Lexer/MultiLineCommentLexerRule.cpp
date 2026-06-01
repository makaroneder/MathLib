#include "MultiLineCommentLexerRule.hpp"

namespace MathLib {
    MultiLineCommentLexerRule::MultiLineCommentLexerRule(size_t type) : LexerRule(type) {}
    void MultiLineCommentLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        const size_t size = str.GetSize();
        if (i + 1 >= size) return;
        if (str.AtUnsafe(i) != '/') return;
        if (str.AtUnsafe(i + 1) != '*') return;
        i += 2;
        while (i + 1 < size) {
            if (str.AtUnsafe(i++) != '*' || str.AtUnsafe(i) != '/') continue;
            i++;
            break;
        }
    }
}