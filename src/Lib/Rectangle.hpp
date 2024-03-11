#ifndef Rectangle_H
#define Rectangle_H
#include "Vector.hpp"
#include <cmath>

template <typename T>
struct Rectangle {
    Vector3<T> position;
    T width;
    T height;

    Rectangle(Vector3<T> pos = Vector3<T>(), T w = 0, T h = 0) {
        position = pos;
        width = w;
        height = h;
    }
    constexpr bool CollidesWith(Rectangle<T> other) const {
        const T x1 = GetVectorAxis(position, VectorAxis::X);
        const T y1 = GetVectorAxis(position, VectorAxis::Y);
        const T x2 = GetVectorAxis(other.position, VectorAxis::X);
        const T y2 = GetVectorAxis(other.position, VectorAxis::Y);
        return y1 + height > y2 && y2 + other.height > y1 && x1 + width > x2 && x2 + other.width > x1;
    }
    constexpr T GetArea(void) const {
        return width * height;
    }
};

#endif