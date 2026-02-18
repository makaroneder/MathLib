#include "Block.hpp"
#include <Grid.hpp>

Block::Block(void) : centerX(SIZE_MAX), centerY(SIZE_MAX), state(State::Dead) {}
Block::Block(ssize_t centerX, ssize_t centerY) : centerX(centerX), centerY(centerY), state(State::Normal) {}
bool Block::Equals(const Block& other) const {
    return centerX == other.centerX && centerY == other.centerY && state == other.state;
}
MathLib::Matrix<Block> ResetBlocks(ssize_t centerX, ssize_t centerY, size_t blockWidth, size_t blockHeight, size_t cellsX, size_t cellsY) {
    const MathLib::Matrix<MathLib::SingleTypePair<ssize_t>> grid = MathLib::GetGridCellsCenter(centerX, centerY, blockWidth, blockHeight, cellsX, cellsY);
    const size_t width = grid.GetWidth();
    const size_t height = grid.GetHeight();
    MathLib::Matrix<Block> ret = MathLib::Matrix<Block>(width, height);
    for (size_t y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++) ret.AtUnsafe(x, y) = Block(grid.AtUnsafe(x, y).first, grid.AtUnsafe(x, y).second);
    ret.AtUnsafe(0, height - 1).state = Block::State::Final;    
    return ret;
}