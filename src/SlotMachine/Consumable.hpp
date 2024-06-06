#ifndef Consumable_H
#define Consumable_H
#include <Geometry/WavefrontObject.hpp>

struct Consumable : WavefrontObject<num_t> {
    enum class Type {
        RewardMultiplier,
        BetIncreaser,
        RewardRankIncreaser,
    } type;
    ssize_t value;
    ssize_t cost;

    Consumable(FileSystem& fileSystem, String path, Type t, ssize_t val, ssize_t cost_);
};

#endif