#include "Collision.hpp"

namespace MathLib {
    bool CheckPartialBoxCollision(size_t x1, size_t x2, size_t radius) {
        return (x1 < x2 + radius) && (x2 < x1 + radius);
    }
    bool CheckBoxCollision(size_t x1, size_t y1, size_t x2, size_t y2, size_t width1, size_t height1, size_t width2, size_t height2) {
        return CheckPartialBoxCollision(x1 * 2, x2 * 2, width1 + width2) && CheckPartialBoxCollision(y1 * 2, y2 * 2, height1 + height2);
    }
}