#include "WhitespaceLexerRule.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    WhitespaceLexerRule::WhitespaceLexerRule(size_t type) : LexerRule(type) {
        EmptyBenchmark
    }
    void WhitespaceLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        StartBenchmark
        const size_t size = str.GetSize();
        while (i < size && IsWhiteSpace(str.At(i))) i++;
        EndBenchmark
    }
}