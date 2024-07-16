#ifndef Particle_H
#define Particle_H
#include "../Geometry/LineShape.hpp"
#include "SIUnits.hpp"

template <typename T>
struct Particle : LineShape<T> {
    Particle(const Matrix<T>& position = CreateVector<T>(0, 0, 0), const bool& fixed = false) : LineShape<T>(position), acceleration(CreateVector<T>(0, 0, 0)), prevPosition(position), fixed(fixed) {}
    constexpr Matrix<T> GetPreviousPosition(void) const {
        return prevPosition;
    }
    constexpr bool IsFixed(void) const {
        return fixed;
    }
    constexpr void Update(const Second<T>& dt) {
        if (!fixed) {
            const Matrix<T> tmp = this->position;
            this->position = this->position * 2 - prevPosition + acceleration * dt.Pow(2).GetValue();
            prevPosition = tmp;
            acceleration = CreateVector<T>(0, 0, 0);
        }
    }
    constexpr void Accelerate(const Matrix<T>& acc) {
        if (!fixed) acceleration += acc;
    }
    virtual Array<Line<T>> ToLines(const Matrix<T>&) const override {
        Array<Line<T>> ret = Array<Line<T>>(1);
        ret.At(0) = Line<T>(prevPosition, this->position);
        return ret;
    }
    virtual bool CollidesWith(const Shape<T>& other_) const override {
        const Particle<T>& other = (const Particle<T>&)other_;
        return this->position == other.position;
    }

    private:
    Matrix<T> acceleration;
    Matrix<T> prevPosition;
    bool fixed;
};

#endif