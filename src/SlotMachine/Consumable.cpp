#include "Consumable.hpp"

Consumable::Consumable(FileSystem& fileSystem, String path, Type t, ssize_t val, ssize_t cost_) : WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), fileSystem, path), type(t), value(val), cost(cost_) {}