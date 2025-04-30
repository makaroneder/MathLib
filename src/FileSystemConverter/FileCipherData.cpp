#include "FileCipherData.hpp"

FileCipherData::FileCipherData(void) : name(), size(SIZE_MAX), type(Type::Invalid) {}
FileCipherData::FileCipherData(Type type, const MathLib::String& name, size_t size) : name(name), size(size), type(type) {}