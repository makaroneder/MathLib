#ifndef Physics_Particle_H
#define Particle_H
#include "../Geometry/LineShape.hpp"
#include "SIUnits.hpp"

namespace MathLib {
    template <typename T>
    struct Particle : LineShape<T> {
        Particle(const Matrix<T>& position = CreateVector<T>(0, 0, 0), bool fixed = false) : LineShape<T>(position), prevPosition(position), acceleration(CreateVector<T>(0, 0, 0)), fixed(fixed) {}
        constexpr Matrix<T> GetPreviousPosition(void) const {
            return prevPosition;
        }
        constexpr bool IsFixed(void) const {
            return fixed;
        }
        virtual void Update(const Second<T>& dt) {
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
            return MakeArrayFromSingle<Line<T>>(Line<T>(prevPosition, this->position));
        }
        virtual bool CollidesWith(const Shape<T>& other_) const override {
            const Particle<T>& other = (const Particle<T>&)other_;
            return this->position == other.position;
        }
        Matrix<T> prevPosition;

        private:
        Matrix<T> acceleration;
        bool fixed;
    };
}

#endif