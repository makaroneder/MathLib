#include "Block.hpp"

Block::Block(void) : file(), x(), y() {}
Block::Block(const MathLib::String& file, ssize_t x, ssize_t y) : file(file), x(x), y(y) {}
bool Block::Equals(const Block& other) const {
    return x == other.x && y == other.y && file == other.file;
}