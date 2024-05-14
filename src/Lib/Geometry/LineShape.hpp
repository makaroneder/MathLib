#ifndef Geometry_LineShape_H
#define Geometry_LineShape_H
#include "Shape.hpp"
#include "Line.hpp"

template <typename T>
struct LineShape : Shape<T> {
    LineShape(Matrix<T> pos) : Shape<T>(pos) {}
    virtual std::vector<Line<T>> ToLines(Matrix<T> rotation) const = 0;
};

#endif