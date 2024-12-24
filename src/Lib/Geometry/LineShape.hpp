#ifndef MathLib_Geometry_LineShape_H
#define MathLib_Geometry_LineShape_H
#include "Shape.hpp"
#include "Line.hpp"

namespace MathLib {
    template <typename T>
    struct LineShape : Shape<T> {
        LineShape(const Matrix<T>& pos) : Shape<T>(pos) {
            EmptyBenchmark
        }
        virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const = 0;
    };
}

#endif