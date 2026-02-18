#ifndef Block_H
#define Block_H
#include <Math/Matrix.hpp>

struct Block;
struct Block : MathLib::Comparable<Block> {
    enum class State : uint8_t {
        Normal,
        Dead,
        Final,
    };
    ssize_t centerX;
    ssize_t centerY;
    State state;

    Block(void);
    Block(ssize_t centerX, ssize_t centerY);

    protected:
    [[nodiscard]] virtual bool Equals(const Block& other) const override;
};
MathLib::Matrix<Block> ResetBlocks(ssize_t centerX, ssize_t centerY, size_t blockWidth, size_t blockHeight, size_t cellsX, size_t cellsY);

#endif