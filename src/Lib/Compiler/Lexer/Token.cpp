#include "Token.hpp"
#include "../../String.hpp"

namespace MathLib {
    Token::Token(void) : value(), type(SIZE_MAX) {}
    Token::Token(size_t type, const Sequence<char>& value) : value(CollectionToString(value)), type(type) {}
    bool Token::CheckType(size_t t) const {
        return type == t;
    }
    String Token::GetValue(void) const {
        return value;
    }
    String Token::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + (type == SIZE_MAX ? "Invalid token" : Formatter<size_t>::ToString(type) + (value.IsEmpty() ? "" : ": "_M + value));
    }
    bool Token::operator==(const Token& other) const {
        return type == other.type && value == other.value;
    }
    bool Token::operator!=(const Token& other) const {
        return !(*this == other);
    }
}