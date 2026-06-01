#ifndef Game_H
#define Game_H
#include "Block.hpp"
#include <Math/Matrix.hpp>
#include <CSV.hpp>

struct Game {
    MathLib::Matrix<Block> blocks;
    MathLib::Array<size_t> keys;
    size_t playerX;
    size_t playerY;

    Game(void);
    Game(const MathLib::CSV& csv, size_t screenWidth, size_t screenHeight, size_t blockWidth, size_t blockHeight);
    void Move(int8_t& dx, int8_t& dy);
};

#endif