#include "LexerRule.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    LexerRule::LexerRule(size_t type) : type(type) {
        EmptyBenchmark
    }
    size_t LexerRule::GetType(void) const {
        StartAndReturnFromBenchmark(type);
    }
}