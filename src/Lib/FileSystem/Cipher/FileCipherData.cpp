#include "FileCipherData.hpp"

namespace MathLib {
    FileCipherData::FileCipherData(void) : name(), size(SIZE_MAX), type(Type::Invalid) {}
    FileCipherData::FileCipherData(Type type, const Sequence<char>& name, size_t size) : name(CollectionToString(name)), size(size), type(type) {}
}