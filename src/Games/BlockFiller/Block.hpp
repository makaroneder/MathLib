#ifndef Block_H
#define Block_H
#include <Image/Animation.hpp>

struct Block;
struct Block : MathLib::Comparable<Block> {
    enum class Type : uint8_t {
        None,
        Empty,
        Coin,
        Filled,
        Wall,
        Stop,
        Left,
        Right,
        Up,
        Down,
        Key,
        Door,
        Teleport,
    };
    MathLib::Animation animation;
    ssize_t x;
    ssize_t y;
    size_t data;
    Type type;
    bool initAnimation;

    Block(void);
    Block(Type type, ssize_t x, ssize_t y);
    [[nodiscard]] virtual bool Equals(const Block& other) const override;
};

#endif