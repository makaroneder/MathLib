#ifndef Geometry_Pyramid_H
#define Geometry_Pyramid_H
#include "LineShape.hpp"
#include "../Quaternion.hpp"

template <typename T>
struct Pyramid : LineShape<T> {
    Matrix<T> sizes;

    Pyramid(Matrix<T> pos, Matrix<T> s) : LineShape<T>(pos), sizes(s) {}
    virtual bool CollidesWith(const Shape<T>& other_) const override {
        // TODO:
        (void)other_;
        return false;
    }
    virtual std::vector<Line<T>> ToLines(Matrix<T> rotation) const override {
        const T sizeX = GetX(sizes) / 2;
        const T sizeY = GetY(sizes) / 2;
        const T sizeZ = GetZ(sizes) / 2;
        Matrix<T> p1 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, -sizeY, sizeZ), this->position, rotation);
        Matrix<T> p2 = RotateVector<T>(this->position + CreateVector<T>(sizeX, -sizeY, sizeZ), this->position, rotation);
        Matrix<T> p3 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, -sizeY, -sizeZ), this->position, rotation);
        Matrix<T> p4 = RotateVector<T>(this->position + CreateVector<T>(sizeX, -sizeY, -sizeZ), this->position, rotation);
        Matrix<T> p5 = RotateVector<T>(this->position + CreateVector<T>(0, sizeY, 0), this->position, rotation);
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