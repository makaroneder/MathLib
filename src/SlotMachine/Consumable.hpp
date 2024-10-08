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

    Consumable(FileSystem& fileSystem, const String& path, Type type, ssize_t value, ssize_t cost);
};

#endif