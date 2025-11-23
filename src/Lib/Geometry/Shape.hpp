#ifndef MathLib_Geometry_Shape_H
#define MathLib_Geometry_Shape_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    template <typename T>
    struct Shape : Allocatable {
        Shape(const Matrix<T>& pos) : position(pos) {}
        [[nodiscard]] virtual bool CollidesWith(const Shape<T>& other) const = 0;
        [[nodiscard]] bool operator==(const Shape<T>& other) const {
            return position == other.position;
        }
        [[nodiscard]] bool operator!=(const Shape<T>& other) const {
            return !(*this == other);
        }

        Matrix<T> position;
    };
}

#endif