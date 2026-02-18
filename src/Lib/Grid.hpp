#ifndef MathLib_Grid_H
#define MathLib_Grid_H
#include "Math/Matrix.hpp"
#include "SingleTypePair.hpp"

namespace MathLib {
    [[nodiscard]] Matrix<SingleTypePair<ssize_t>> GetGridCellsCenter(ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t cellsX, size_t cellsY);
}

#endif