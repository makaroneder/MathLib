#ifndef Geometry_Cuboid_H
#define Geometry_Cuboid_H
#include "LineShape.hpp"
#include "../Quaternion.hpp"

template <typename T>
struct Cuboid : LineShape<T> {
    Matrix<T> sizes;

    Cuboid(const Matrix<T>& pos = CreateVector<T>(0, 0, 0), const Matrix<T>& s = CreateVector<T>(0, 0, 0)) : LineShape<T>(pos), sizes(s) {}
    virtual bool CollidesWith(const Shape<T>& other_) const override {
        const Cuboid<T>& other = (const Cuboid<T>&)other_;
        for (size_t x = 0; x < this->position.GetWidth(); x++)
            if ((this->position.At(x, 0) + sizes.At(x, 0) <= other.position.At(x, 0)) || (other.position.At(x, 0) + other.sizes.At(x, 0) <= this->position.At(x, 0))) return false;
        return true;
    }
    virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
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
        Array<Line<T>> ret = Array<Line<T>>(12);
        ret.At(0) = Line<T>(p1, p2);
        ret.At(1) = Line<T>(p1, p3);
        ret.At(2) = Line<T>(p2, p4);
        ret.At(3) = Line<T>(p3, p4);
        ret.At(4) = Line<T>(p1, p5);
        ret.At(5) = Line<T>(p2, p6);
        ret.At(6) = Line<T>(p3, p7);
        ret.At(7) = Line<T>(p4, p8);
        ret.At(8) = Line<T>(p5, p6);
        ret.At(9) = Line<T>(p5, p7);
        ret.At(10) = Line<T>(p6, p8);
        ret.At(11) = Line<T>(p7, p8);
        return ret;
    }
};

#endif