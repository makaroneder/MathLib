#include "Token.hpp"
#include <String.hpp>

Token::Token(void) : value(), type(SIZE_MAX) {}
Token::Token(size_t type, const MathLib::Sequence<char>& value) : value(MathLib::CollectionToString(value)), type(type) {}
bool Token::CheckType(size_t t) const {
    return type == t;
}
MathLib::String Token::GetValue(void) const {
    return value;
}
MathLib::String Token::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + (type == SIZE_MAX ? "Invalid token" : MathLib::ToString(type, 10) + (value.IsEmpty() ? "" : ": "_M + value));
}
bool Token::operator==(const Token& other) const {
    return type == other.type && value == other.value;
}
bool Token::operator!=(const Token& other) const {
    return !(*this == other);
}