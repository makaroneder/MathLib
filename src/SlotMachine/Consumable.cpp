#include "Consumable.hpp"

Consumable::Consumable(FileSystem& fileSystem, const String& path, const Type& t, const ssize_t& val, const ssize_t& cost_) : WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), fileSystem, path), type(t), value(val), cost(cost_) {}