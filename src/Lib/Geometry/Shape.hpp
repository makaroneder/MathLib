#ifndef Geometry_Shape_H
#define Geometry_Shape_H
#include "../Matrix.hpp"

template <typename T>
struct Shape {
    Shape(Matrix<T> pos) : position(pos) {}
    virtual ~Shape(void) {}
    virtual bool CollidesWith(const Shape<T>& other) const = 0;

    Matrix<T> position;
};

#endif