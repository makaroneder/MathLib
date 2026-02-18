#ifndef MathLib_Collision_H
#define MathLib_Collision_H
#include <stddef.h>

namespace MathLib {
    [[nodiscard]] bool CheckPartialBoxCollision(size_t x1, size_t x2, size_t radius);
    [[nodiscard]] bool CheckBoxCollision(size_t x1, size_t y1, size_t x2, size_t y2, size_t width1, size_t height1, size_t width2, size_t height2);
}

#endif