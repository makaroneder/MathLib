#ifndef Particle_H
#define Particle_H
#include <Physics/SIUnits.hpp>
#include <Geometry/Shape.hpp>
#include <Quaternion.hpp>

template <typename T>
struct Particle : Shape<T> {
    Particle(Matrix<T> pos, Kilogram<T> m, bool f) : Shape<T>(pos), mass(m), velocity(CreateVector<T>(0, 0, 0)), prevPosition(pos), fixed(f) {}
    constexpr void SetPosition(Matrix<T> pos) {
        const Matrix<T> tmp = this->position;
        this->position = pos;
        prevPosition = tmp;
    }
    constexpr Matrix<T> GetPreviousPosition(void) const {
        return prevPosition;
    }
    constexpr bool IsFixed(void) const {
        return fixed;
    }
    constexpr Kilogram<T> GetMass(void) const {
        return mass;
    }
    constexpr Matrix<T> GetNewPosition(Second<T> dt) const {
        return velocity * dt.GetValue();
    }
    constexpr void Update(Second<T> dt, Matrix<T> force) {
        velocity += (force / mass.GetValue()) * dt.GetValue();
    }
    virtual std::vector<Line<T>> ToLines(Matrix<T> rotation) const override {
        (void)rotation;
        return { Line<T>(this->position, this->position), };
    }

    private:
    Kilogram<T> mass;
    Matrix<T> velocity;
    Matrix<T> prevPosition;
    bool fixed;
};

#endif