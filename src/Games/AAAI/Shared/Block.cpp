#include "Block.hpp"

Block::Block(void) : path(), x(), y() {}
Block::Block(ssize_t x, ssize_t y) : path(), x(x), y(y) {}
Block::Block(const MathLib::String& path, ssize_t x, ssize_t y) : path(path), x(x), y(y) {}
bool Block::LessThanEqual(const Block& other) const {
    if (x < other.x) return true;
    return x == other.x && y <= other.y;
}
bool Block::Save(MathLib::Writable& file) const {
    return file.Write<ssize_t>(x) && file.Write<ssize_t>(y) && file.Puts(path) + file.Write<char>('\0');
}
bool Block::Load(MathLib::Readable& file) {
    if (!file.Read<ssize_t>(x) || !file.Read<ssize_t>(y)) return false;
    path = file.ReadUntil('\0');
    return true;
}