#ifndef MathLib_Geometry_Shape_H
#define MathLib_Geometry_Shape_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    template <typename T>
    struct Shape;
    template <typename T>
    struct Shape : Comparable<Shape<T>> {
        Matrix<T> position;

        Shape(const Matrix<T>& pos) : position(pos) {}
        [[nodiscard]] virtual bool CollidesWith(const Shape<T>& other) const = 0;

        protected:
        [[nodiscard]] virtual bool Equals(const Shape<T>& other) const override {
            return position == other.position;
        }
    };
}

#endif