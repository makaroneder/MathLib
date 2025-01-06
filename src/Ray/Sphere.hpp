#ifndef Shpere_H
#define Sphere_H
#include <Geometry/Ray.hpp>

template <typename T>
struct Sphere : MathLib::Shape<T> {
    Sphere(const MathLib::Matrix<T>& pos, const T& radius) : MathLib::Shape<T>(pos), radius(radius) {}
    [[nodiscard]] virtual bool CollidesWith(const MathLib::Shape<T>& other) const override {
        // TODO:
        (void)other;
        return false;
    }
    [[nodiscard]] bool HasIntersections(const MathLib::Ray<T>& ray) const {
        const MathLib::Matrix<T> tmp = ray.position - this->position;
        return 4 * (MathLib::Pow(ray.GetDirection().Dot(tmp), 2) - tmp.GetLengthSquared() + radius * radius) >= 0;
    }

    private:
    T radius;
};

#endif