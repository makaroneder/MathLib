#ifndef Geometry_Cuboid_H
#define Geometry_Cuboid_H
#include "Shape.hpp"
#include "../Quaternion.hpp"

template <typename T>
struct Cuboid : Shape<T> {
    Matrix<T> position;
    Matrix<T> sizes;

    Cuboid(Matrix<T> pos = CreateVector<T>(0, 0, 0), Matrix<T> s = CreateVector<T>(0, 0, 0)) {
        position = pos;
        sizes = s;
    }
    virtual std::vector<Line<T>> ToLines(T angle, Matrix<T> axis) const override {
        const T sizeX = GetX(sizes) / 2;
        const T sizeY = GetY(sizes) / 2;
        const T sizeZ = GetZ(sizes) / 2;
        const Matrix<T> p1 = RotateVector<T>(position + CreateVector<T>(sizeX, sizeY, sizeZ), position, angle, axis);
        const Matrix<T> p2 = RotateVector<T>(position + CreateVector<T>(sizeX, -sizeY, sizeZ), position, angle, axis);
        const Matrix<T> p3 = RotateVector<T>(position + CreateVector<T>(-sizeX, sizeY, sizeZ), position, angle, axis);
        const Matrix<T> p4 = RotateVector<T>(position + CreateVector<T>(-sizeX, -sizeY, sizeZ), position, angle, axis);
        const Matrix<T> p5 = RotateVector<T>(position + CreateVector<T>(sizeX, sizeY, -sizeZ), position, angle, axis);
        const Matrix<T> p6 = RotateVector<T>(position + CreateVector<T>(sizeX, -sizeY, -sizeZ), position, angle, axis);
        const Matrix<T> p7 = RotateVector<T>(position + CreateVector<T>(-sizeX, sizeY, -sizeZ), position, angle, axis);
        const Matrix<T> p8 = RotateVector<T>(position + CreateVector<T>(-sizeX, -sizeY, -sizeZ), position, angle, axis);
        return {
            Line<T>(p1, p2), Line<T>(p1, p3), Line<T>(p2, p4), Line<T>(p3, p4),
            Line<T>(p1, p5), Line<T>(p2, p6), Line<T>(p3, p7), Line<T>(p4, p8),
            Line<T>(p5, p6), Line<T>(p5, p7), Line<T>(p6, p8), Line<T>(p7, p8),
        };
    }
    constexpr bool CollidesWith(Cuboid<T> other) const {
        for (size_t x = 0; x < position.GetWidth(); x++)
            if ((position.At(x, 0) + sizes.At(x, 0) <= other.position.At(x, 0)) || (other.position.At(x, 0) + other.sizes.At(x, 0) <= position.At(x, 0))) return false;
        return true;
    }
};

#endif