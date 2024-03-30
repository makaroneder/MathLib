#ifndef Geometry_Shape_H
#define Geometry_Shape_H
#include "Line.hpp"

template <typename T>
struct Shape {
    virtual ~Shape(void) {}
    virtual std::vector<Line<T>> ToLines(T angle, Matrix<T> axis) const = 0;
};

#endif