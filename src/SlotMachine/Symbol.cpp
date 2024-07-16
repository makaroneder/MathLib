#include "Symbol.hpp"

Symbol::Symbol(FileSystem& fileSystem, const String& path, const String& n, const Array<size_t>& mult) : WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), fileSystem, path), name(n), multiplier(mult) {}
bool Symbol::operator==(const Symbol& other) const {
    return name == other.name;
}