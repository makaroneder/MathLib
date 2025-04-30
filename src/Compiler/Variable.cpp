#include "Variable.hpp"

Variable::Variable(void) : name(), type(SIZE_MAX), offset(SIZE_MAX) {}
Variable::Variable(const MathLib::Sequence<char>& name) : name(MathLib::CollectionToString(name)), type(SIZE_MAX), offset(SIZE_MAX) {}
Variable::Variable(const MathLib::Sequence<char>& name, size_t type, size_t offset) : name(MathLib::CollectionToString(name)), type(type), offset(offset) {}
bool Variable::operator==(const Variable& other) const {
    return name == other.name;
}
bool Variable::operator!=(const Variable& other) const {
    return !(*this == other);
}