#ifndef Geometry_Cuboid_H
#define Geometry_Cuboid_H
#include "LineShape.hpp"
#include "../Quaternion.hpp"

template <typename T>
struct Cuboid : LineShape<T> {
    Matrix<T> sizes;

    Cuboid(Matrix<T> pos = CreateVector<T>(0, 0, 0), Matrix<T> s = CreateVector<T>(0, 0, 0)) : LineShape<T>(pos), sizes(s) {}
    virtual bool CollidesWith(const Shape<T>& other_) const override {
        const Cuboid<T>& other = (const Cuboid<T>&)other_;
        for (size_t x = 0; x < this->position.GetWidth(); x++)
            if ((this->position.At(x, 0) + sizes.At(x, 0) <= other.position.At(x, 0)) || (other.position.At(x, 0) + other.sizes.At(x, 0) <= this->position.At(x, 0))) return false;
        return true;
    }
    virtual std::vector<Line<T>> ToLines(Matrix<T> rotation) const override {
        const T sizeX = GetX(sizes) / 2;
        const T sizeY = GetY(sizes) / 2;
        const T sizeZ = GetZ(sizes) / 2;
        const Matrix<T> p1 = RotateVector<T>(this->position + CreateVector<T>(sizeX, sizeY, sizeZ), this->position, rotation);
        const Matrix<T> p2 = RotateVector<T>(this->position + CreateVector<T>(sizeX, -sizeY, sizeZ), this->position, rotation);
        const Matrix<T> p3 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, sizeY, sizeZ), this->position, rotation);
        const Matrix<T> p4 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, -sizeY, sizeZ), this->position, rotation);
        const Matrix<T> p5 = RotateVector<T>(this->position + CreateVector<T>(sizeX, sizeY, -sizeZ), this->position, rotation);
        const Matrix<T> p6 = RotateVector<T>(this->position + CreateVector<T>(sizeX, -sizeY, -sizeZ), this->position, rotation);
        const Matrix<T> p7 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, sizeY, -sizeZ), this->position, rotation);
        const Matrix<T> p8 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, -sizeY, -sizeZ), this->position, rotation);
        return {
            Line<T>(p1, p2), Line<T>(p1, p3), Line<T>(p2, p4), Line<T>(p3, p4),
            Line<T>(p1, p5), Line<T>(p2, p6), Line<T>(p3, p7), Line<T>(p4, p8),
            Line<T>(p5, p6), Line<T>(p5, p7), Line<T>(p6, p8), Line<T>(p7, p8),
        };
    }
};

#endif