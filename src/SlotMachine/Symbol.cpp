#include "Symbol.hpp"

Symbol::Symbol(FileSystem& fileSystem, const String& path, const String& name, const Array<size_t>& multiplier) : WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), fileSystem, path), name(name), multiplier(multiplier) {}
bool Symbol::operator==(const Symbol& other) const {
    return name == other.name;
}