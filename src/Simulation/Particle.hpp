#ifndef Particle_H
#define Particle_H
#include <Physics/SIUnits.hpp>
#include <Geometry/LineShape.hpp>
#include <Quaternion.hpp>

template <typename T>
struct Particle : LineShape<T> {
    Particle(Matrix<T> pos, Kilogram<T> m, bool f) : LineShape<T>(pos), mass(m), velocity(CreateVector<T>(0, 0, 0)), prevPosition(pos), fixed(f) {}
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
    virtual Array<Line<T>> ToLines(Matrix<T> rotation) const override {
        (void)rotation;
        Array<Line<T>> ret = Array<Line<T>>(1);
        ret.At(0) = Line<T>(this->position, this->position);
        return ret;
    }
    virtual bool CollidesWith(const Shape<T>&) const override {
        // TODO:
        return false;
    }

    private:
    Kilogram<T> mass;
    Matrix<T> velocity;
    Matrix<T> prevPosition;
    bool fixed;
};

#endif