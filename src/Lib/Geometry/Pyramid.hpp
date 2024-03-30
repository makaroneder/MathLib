#ifndef Geometry_Pyramid_H
#define Geometry_Pyramid_H
#include "Shape.hpp"
#include "../Quaternion.hpp"

template <typename T>
struct Pyramid : Shape<T> {
    Matrix<T> position;
    Matrix<T> sizes;

    Pyramid(Matrix<T> pos, Matrix<T> s) {
        position = pos;
        sizes = s;
    }
    virtual std::vector<Line<T>> ToLines(T angle, Matrix<T> axis) const override {
        const T sizeX = GetX(sizes) / 2;
        const T sizeY = GetY(sizes) / 2;
        const T sizeZ = GetZ(sizes) / 2;
        Matrix<T> p1 = RotateVector<T>(position + CreateVector<T>(-sizeX, -sizeY, sizeZ), position, angle, axis);
        Matrix<T> p2 = RotateVector<T>(position + CreateVector<T>(sizeX, -sizeY, sizeZ), position, angle, axis);
        Matrix<T> p3 = RotateVector<T>(position + CreateVector<T>(-sizeX, -sizeY, -sizeZ), position, angle, axis);
        Matrix<T> p4 = RotateVector<T>(position + CreateVector<T>(sizeX, -sizeY, -sizeZ), position, angle, axis);
        Matrix<T> p5 = RotateVector<T>(position + CreateVector<T>(0, sizeY, 0), position, angle, axis);
        return {
            Line<T>(p1, p2),
            Line<T>(p1, p3),
            Line<T>(p2, p4),
            Line<T>(p3, p4),
            Line<T>(p1, p5),
            Line<T>(p2, p5),
            Line<T>(p3, p5),
            Line<T>(p4, p5),
        };
    }
};

#endif