#ifndef Lens_H
#define Lens_H
#include "Intersectable.hpp"

template <typename T>
struct Lens : Intersectable<T> {
    T radius;

    Lens(const MathLib::Matrix<T>& position, const T& refractionIndex, const T& radius) : Intersectable<T>(position, refractionIndex), radius(radius) {}
    virtual MathLib::Expected<T> GetIntersection(const MathLib::Ray<T>& ray) const override {
        const MathLib::Matrix<T> oc = ray.position - this->position;
        const T a = ray.GetDirection().Dot(ray.GetDirection());
        const T b = oc.Dot(ray.GetDirection()) * 2;
        const T delta = b * b - 4 * a * (oc.Dot(oc) - radius * radius);
        if (delta < 0) return MathLib::Expected<T>();
        const T t1 = (-b - MathLib::Sqrt(delta)) / (a * 2);
        const T t2 = (-b + MathLib::Sqrt(delta)) / (a * 2);
        if (t1 < 0) return t2 < 0 ? MathLib::Expected<T>() : MathLib::Expected<T>(t2);
        else if (t2 < 0) return MathLib::Expected<T>(t1);
        else return MathLib::Expected<T>(MathLib::Min<T>(t1, t2));        
    }
};

#endif