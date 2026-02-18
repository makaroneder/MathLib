#ifndef Intersectable_H
#define Intersectable_H
#include <Geometry/Ray.hpp>

template <typename T>
struct Intersectable {
    MathLib::Matrix<T> position;

    Intersectable(const MathLib::Matrix<T>& position) : position(position) {}
    virtual MathLib::Expected<T> GetIntersection(const MathLib::Ray<T>& ray) const = 0;
    virtual MathLib::Ray<T> IntersectionToRay(const MathLib::Matrix<T>& dir, const MathLib::Matrix<T>& intersection, const T& refIndex, bool isInsideLens) const = 0;
};

#endif