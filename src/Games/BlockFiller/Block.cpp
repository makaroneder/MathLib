#include "Block.hpp"

Block::Block(void) : animation(), x(SIZE_MAX), y(SIZE_MAX), data(SIZE_MAX), type(Type::None), initAnimation(false) {}
Block::Block(Type type, ssize_t x, ssize_t y) : animation(), x(x), y(y), data(0), type(type), initAnimation(false) {}
bool Block::Equals(const Block& other) const {
    return type == other.type && x == other.x && y == other.y;
}