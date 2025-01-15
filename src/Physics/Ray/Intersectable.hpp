#ifndef Intersectable_H
#define Intersectable_H
#include <Geometry/Ray.hpp>

template <typename T>
struct Intersectable {
    MathLib::Matrix<T> position;
    T refractionIndex;

    Intersectable(const MathLib::Matrix<T>& position, const T& refractionIndex) : position(position), refractionIndex(refractionIndex) {}
    virtual MathLib::Expected<T> GetIntersection(const MathLib::Ray<T>& ray) const = 0;
};

#endif