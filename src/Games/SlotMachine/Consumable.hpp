#ifndef Consumable_H
#define Consumable_H
#include <Geometry/WavefrontObject.hpp>

struct Consumable : MathLib::WavefrontObject<MathLib::num_t> {
    enum class Type {
        RewardMultiplier,
        BetIncreaser,
        RewardRankIncreaser,
    } type;
    ssize_t value;
    ssize_t cost;

    Consumable(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& path, Type type, ssize_t value, ssize_t cost);
};

#endif