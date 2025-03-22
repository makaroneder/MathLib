#include "Consumable.hpp"

Consumable::Consumable(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& path, Type type, ssize_t value, ssize_t cost) : MathLib::WavefrontObject<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0)), type(type), value(value), cost(cost) {
    if (!LoadFromPath(fileSystem, path)) MathLib::Panic("Failed to load wavefront object");
}