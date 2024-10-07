#include "Consumable.hpp"

Consumable::Consumable(FileSystem& fileSystem, const String& path, Type type, ssize_t value, ssize_t cost) : WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), fileSystem, path), type(type), value(value), cost(cost) {}