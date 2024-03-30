#ifndef Particle_H
#define Particle_H
#include <Physics/SIUnits.hpp>
#include <Matrix.hpp>

template <typename T>
struct Particle {
    Particle(Matrix<T> pos, Matrix<T> vel, bool f) {
        position = prevPosition = pos;
        velocity = vel;
        fixed = f;
    }
    constexpr void SetPosition(Matrix<T> pos) {
        const Matrix<T> tmp = position;
        position = pos;
        prevPosition = tmp;
    }
    constexpr Matrix<T> GetPreviousPosition(void) const {
        return prevPosition;
    }
    constexpr bool IsFixed(void) const {
        return fixed;
    }
    constexpr Matrix<T> GetNewPosition(Second<T> time) const {
        return velocity * time.GetValue();
    }
    Matrix<T> position;

    private:
    Matrix<T> velocity;
    Matrix<T> prevPosition;
    bool fixed;
};

#endif