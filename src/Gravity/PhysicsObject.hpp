#ifndef PhysicsObject_H
#define PhysicsObject_H
#include <Physics/Constants.hpp>
#include <Geometry/Cuboid.hpp>

template <typename T>
struct PhysicsObject : Cuboid<T> {
    PhysicsObject(Matrix<T> pos, Matrix<T> s, Kilogram<T> m, Matrix<T> v) : Cuboid<T>(pos, s), mass(m), velocity(v) {}
    void ApplyForce(Matrix<T> force, Second<T> dt) {
        velocity += force / mass.GetValue() * dt.GetValue();
        this->position += velocity * dt.GetValue();
    }
    void ApplyGravity(PhysicsObject<T>& other, Second<T> dt) {
        const Matrix<T> dist = other.position - this->position;
        const Matrix<T> tmp = dist.Normalize() * ((constantOfGravitation * mass * other.mass / Metre<num_t>(dist.GetLength()).Pow(2)) * dt).GetValue();
        ApplyForce(tmp, dt);
        other.ApplyForce(-tmp, dt);
    }

    private:
    Kilogram<T> mass;
    Matrix<T> velocity;
};

#endif