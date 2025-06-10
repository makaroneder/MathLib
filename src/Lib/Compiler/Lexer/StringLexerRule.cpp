#include "StringLexerRule.hpp"

namespace MathLib {
    StringLexerRule::StringLexerRule(size_t type, const Sequence<char>& value) : LexerRule(type), value(CollectionToString(value)) {
        EmptyBenchmark
    }
    void StringLexerRule::Match(const Sequence<char>& str, size_t& i) const {
        StartBenchmark
        const size_t size = value.GetSize();
        if (str.GetSize() < i + size) return;
        for (size_t j = 0; j < size; j++)
            if (str.At(i + j) != value.At(j)) return;
        i += size;
        EndBenchmark
    }
}