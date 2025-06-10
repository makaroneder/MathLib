#include "HTMLAttribute.hpp"

HTMLAttribute::HTMLAttribute(void) : name(), value() {}
HTMLAttribute::HTMLAttribute(const MathLib::Sequence<char>& name, const MathLib::Sequence<char>& value) : name(MathLib::CollectionToString(name)), value(MathLib::CollectionToString(value)) {}
MathLib::String HTMLAttribute::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + name + " = \"" + value + '"';
}
bool HTMLAttribute::Equals(const MathLib::Comparable& other_) const {
    const HTMLAttribute& other = (const HTMLAttribute&)other_;
    return name == other.name && value == other.value;
}