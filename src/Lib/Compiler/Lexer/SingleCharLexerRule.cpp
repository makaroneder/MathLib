#include "SingleCharLexerRule.hpp"

namespace MathLib {
    SingleCharLexerRule::SingleCharLexerRule(size_t type, const Sequence<char>& chars) : LexerRule(type), chars(CollectionToArray<char>(chars)) {
        EmptyBenchmark
    }
    void SingleCharLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        StartBenchmark
        i += i < str.GetSize() && chars.Contains(str.At(i));
        EndBenchmark
    }
}