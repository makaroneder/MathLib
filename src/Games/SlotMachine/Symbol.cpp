#include "Symbol.hpp"

Symbol::Symbol(MathLib::FileSystem& fileSystem, const MathLib::String& path, const MathLib::String& name, const MathLib::Array<size_t>& multiplier) : MathLib::WavefrontObject<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0)), name(name), multiplier(multiplier) {
    if (!LoadFromPath(fileSystem, path)) MathLib::Panic("Failed to load wavefront object");
}
bool Symbol::operator==(const Symbol& other) const {
    return name == other.name;
}