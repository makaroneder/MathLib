#ifndef Geometry_LineShape_H
#define Geometry_LineShape_H
#include "Shape.hpp"
#include "Line.hpp"

namespace MathLib {
    template <typename T>
    struct LineShape : Shape<T> {
        LineShape(const Matrix<T>& pos) : Shape<T>(pos) {}
        virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const = 0;
    };
}

#endif