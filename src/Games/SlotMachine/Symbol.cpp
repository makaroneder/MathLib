#include "Symbol.hpp"

Symbol::Symbol(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& path, const MathLib::Sequence<char>& name, const MathLib::Sequence<size_t>& multiplier) : MathLib::WavefrontObject<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0)), name(MathLib::CollectionToString(name)), multiplier(MathLib::CollectionToArray<size_t>(multiplier)) {
    if (!LoadFromPath(fileSystem, path)) MathLib::Panic("Failed to load wavefront object");
}
bool Symbol::operator==(const Symbol& other) const {
    return name == other.name;
}