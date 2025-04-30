#include "Type.hpp"

Type::Type(void) : name(), size(SIZE_MAX) {}
Type::Type(const MathLib::Sequence<char>& name) : name(MathLib::CollectionToString(name)), size(SIZE_MAX) {}
Type::Type(const MathLib::Sequence<char>& name, size_t size) : name(MathLib::CollectionToString(name)), size(size) {}
bool Type::operator==(const Type& other) const {
    return name == other.name;
}
bool Type::operator!=(const Type& other) const {
    return !(*this == other);
}