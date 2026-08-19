#ifndef Point_H
#define Point_H
#include <Interfaces/Comparable.hpp>

template <typename T>
struct Point;
template <typename T>
struct Point : MathLib::Comparable<Point<T>> {
    T x;
    T y;

    Point(void) : x(), y() {}
    Point(const T& x, const T& y) : x(x), y(y) {}
    [[nodiscard]] virtual bool Equals(const Point<T>& other) const override {
        return x == other.x && y == other.y;
    }
};

#endif