#include "Grid.hpp"

namespace MathLib {
    Matrix<SingleTypePair<ssize_t>> GetGridCellsCenter(ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t cellsX, size_t cellsY) {
        Matrix<SingleTypePair<ssize_t>> ret = Matrix<SingleTypePair<ssize_t>>(cellsX, cellsY);
        const ssize_t leftX = centerX - width * cellsX / 2;
        const ssize_t topY = centerY - height * cellsY / 2;
        for (size_t y = 0; y < cellsY; y++) {
            const ssize_t tmp = topY + y * height + height / 2;
            for (size_t x = 0; x < cellsX; x++) ret.AtUnsafe(x, y) = SingleTypePair<ssize_t>(leftX + x * width + width / 2, tmp);
        }
        return ret;
    }
}