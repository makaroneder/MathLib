#ifndef Object_H
#define Object_H
#include <Renderer.hpp>
#include <Geometry/Cuboid.hpp>
#include <Physics/SIUnits.hpp>

template <typename T>
struct Object {
    Object(Cuboid<T> shape = Cuboid<T>(), MetrePerSecond<T> vel = MetrePerSecond<T>(), uint32_t c = 0) {
        cuboid = shape;
        velocity = vel;
        color = c;
    }
    bool Draw(Renderer& renderer) const {
        return renderer.DrawShape<T>(cuboid, 0, CreateVector<T>(0, 0, 0), color);
    }

    Cuboid<T> cuboid;
    MetrePerSecond<T> velocity;
    uint32_t color;
};

#endif