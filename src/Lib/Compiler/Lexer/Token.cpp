#include "Token.hpp"
#include "../../String.hpp"

namespace MathLib {
    Token::Token(void) : value(), type(SIZE_MAX) {
        EmptyBenchmark
    }
    Token::Token(size_t type, const Sequence<char>& value) : value(CollectionToString(value)), type(type) {
        EmptyBenchmark
    }
    bool Token::CheckType(size_t t) const {
        StartAndReturnFromBenchmark(type == t);
    }
    String Token::GetValue(void) const {
        StartAndReturnFromBenchmark(value);
    }
    String Token::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + (type == SIZE_MAX ? "Invalid token" : MathLib::ToString(type, 10) + (value.IsEmpty() ? "" : ": "_M + value)));
    }
    bool Token::operator==(const Token& other) const {
        StartAndReturnFromBenchmark(type == other.type && value == other.value);
    }
    bool Token::operator!=(const Token& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}