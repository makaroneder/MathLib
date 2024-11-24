#ifndef Geometry_Shape_H
#define Geometry_Shape_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    template <typename T>
    struct Shape : Allocatable {
        Shape(const Matrix<T>& pos) : position(pos) {}
        virtual bool CollidesWith(const Shape<T>& other) const = 0;

        Matrix<T> position;
    };
}

#endif