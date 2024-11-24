#ifndef Geometry_Pyramid_H
#define Geometry_Pyramid_H
#include "LineShape.hpp"
#include "../Math/Quaternion.hpp"

namespace MathLib {
    template <typename T>
    struct Pyramid : LineShape<T> {
        Matrix<T> sizes;

        Pyramid(const Matrix<T>& pos, const Matrix<T>& s) : LineShape<T>(pos), sizes(s) {}
        virtual bool CollidesWith(const Shape<T>&) const override {
            // TODO:
            return false;
        }
        virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
            const T sizeX = GetX(sizes) / 2;
            const T sizeY = GetY(sizes) / 2;
            const T sizeZ = GetZ(sizes) / 2;
            Matrix<T> p1 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, -sizeY, sizeZ), this->position, rotation);
            Matrix<T> p2 = RotateVector<T>(this->position + CreateVector<T>(sizeX, -sizeY, sizeZ), this->position, rotation);
            Matrix<T> p3 = RotateVector<T>(this->position + CreateVector<T>(-sizeX, -sizeY, -sizeZ), this->position, rotation);
            Matrix<T> p4 = RotateVector<T>(this->position + CreateVector<T>(sizeX, -sizeY, -sizeZ), this->position, rotation);
            Matrix<T> p5 = RotateVector<T>(this->position + CreateVector<T>(0, sizeY, 0), this->position, rotation);
            Array<Line<T>> ret = Array<Line<T>>(8);
            ret.At(0) = Line<T>(p1, p2);
            ret.At(1) = Line<T>(p1, p3);
            ret.At(2) = Line<T>(p2, p4);
            ret.At(3) = Line<T>(p3, p4);
            ret.At(4) = Line<T>(p1, p5);
            ret.At(5) = Line<T>(p2, p5);
            ret.At(6) = Line<T>(p3, p5);
            ret.At(7) = Line<T>(p4, p5);
            return ret;
        }
    };
}

#endif