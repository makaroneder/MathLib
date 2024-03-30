#ifndef Entity_H
#define Entity_H
#include "Object.hpp"

template <typename T>
struct Entity : Object<T> {
    Entity(Cuboid<T> cube, MetrePerSecond<T> vel, uint32_t c) : Object<T>(cube, vel, c) {}
    void Update(Second<T> time, Object<T> target) {
        this->cuboid.position += (target.cuboid.position - this->cuboid.position).Normalize() * (this->velocity * time).GetValue();
    }
};

#endif