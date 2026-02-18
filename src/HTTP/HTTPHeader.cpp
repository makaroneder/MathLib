#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader(const MathLib::Sequence<char>& name, const MathLib::Sequence<char>& value) : name(MathLib::CollectionToString(name)), value(MathLib::CollectionToString(value)) {}
MathLib::String HTTPHeader::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + name + ": " + value;
}
bool HTTPHeader::operator==(const HTTPHeader& other) const {
    return name == other.name && value == other.value;
}
bool HTTPHeader::operator!=(const HTTPHeader& other) const {
    return !(*this == other);
}