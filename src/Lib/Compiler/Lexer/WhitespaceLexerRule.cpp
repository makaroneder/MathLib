#include "WhitespaceLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    WhitespaceLexerRule::WhitespaceLexerRule(size_t type) : LexerRule(type) {}
    void WhitespaceLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        SkipWhiteSpace(str, i);
    }
}