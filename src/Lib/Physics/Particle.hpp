#ifndef MathLib_Physics_Particle_H
#define MathLib_Physics_Particle_H
#include "../Geometry/LineShape.hpp"
#include "SIUnits.hpp"

namespace MathLib {
    template <typename T>
    struct Particle : LineShape<T> {
        Particle(const Matrix<T>& position = CreateVector<T>(0, 0, 0), bool fixed = false) : LineShape<T>(position), prevPosition(position), acceleration(CreateVector<T>(0, 0, 0)), fixed(fixed) {
            EmptyBenchmark
        }
        [[nodiscard]] Matrix<T> GetPreviousPosition(void) const {
            StartAndReturnFromBenchmark(prevPosition);
        }
        [[nodiscard]] bool IsFixed(void) const {
            StartAndReturnFromBenchmark(fixed);
        }
        virtual void Update(const Second<T>& dt) {
            StartBenchmark
            if (!fixed) {
                const Matrix<T> tmp = this->position;
                this->position = this->position * 2 - prevPosition + acceleration * dt.Pow(2).GetValue();
                prevPosition = tmp;
                acceleration = CreateVector<T>(0, 0, 0);
            }
            EndBenchmark
        }
        void Accelerate(const Matrix<T>& acc) {
            StartBenchmark
            if (!fixed) acceleration += acc;
            EndBenchmark
        }
        [[nodiscard]] Matrix<T> GetAcceleration(void) const {
            return acceleration;
        }
        [[nodiscard]] virtual Array<Line<T>> ToLines(const Matrix<T>&) const override {
            StartAndReturnFromBenchmark(MakeArray<Line<T>>(Line<T>(prevPosition, this->position)));
        }
        [[nodiscard]] virtual bool CollidesWith(const Shape<T>& other) const override {
            StartAndReturnFromBenchmark(this->position == ((const Particle<T>&)other).position);
        }
        [[nodiscard]] bool operator==(const Particle<T>& other) const {
            StartAndReturnFromBenchmark(Shape<T>::operator==(other) && fixed == other.fixed && prevPosition == other.prevPosition && acceleration == other.acceleration);
        }
        [[nodiscard]] bool operator!=(const Particle<T>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }

        Matrix<T> prevPosition;

        private:
        Matrix<T> acceleration;
        bool fixed;
    };
}

#endif