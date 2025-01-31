#ifndef Refractable_H
#define Refractable_H
#include "Intersectable.hpp"

template <typename T>
MathLib::Matrix<T> Refract(const MathLib::Matrix<T>& direction, const MathLib::Matrix<T>& normal, T n1, T n2) {
    const T n = n1 / n2;
    const T cosI = -normal.Dot(direction);
    const T sinT2 = n * n * (1 - cosI * cosI);
    if (sinT2 > 1) return MathLib::CreateVector<T>(0, 0, 0);
    return direction * n + normal * (n * cosI - MathLib::Sqrt(1 - sinT2));
}
template <typename T>
struct Refractable : Intersectable<T> {
    Refractable(const MathLib::Matrix<T>& position, const T& refractionIndex) : Intersectable<T>(position), refractionIndex(refractionIndex) {}
    virtual MathLib::Matrix<T> GetNormal(const MathLib::Matrix<T>& intersection) const = 0;
    virtual MathLib::Ray<T> IntersectionToRay(const MathLib::Matrix<T>& dir, const MathLib::Matrix<T>& intersection, const T& refIndex, bool isInsideLens) const override {
        const MathLib::Matrix<T> normal = GetNormal(intersection);
        T n1 = refIndex;
        T n2 = refractionIndex;
        if (isInsideLens) MathLib::Swap<T>(n1, n2);
        return MathLib::Ray<T>(intersection, Refract<T>(dir, isInsideLens ? -normal : normal, n1, n2).Normalize());
    }

    private:
    T refractionIndex;
};

#endif