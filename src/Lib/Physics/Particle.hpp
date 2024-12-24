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
        Matrix<T> GetPreviousPosition(void) const {
            StartBenchmark
            ReturnFromBenchmark(prevPosition);
        }
        bool IsFixed(void) const {
            StartBenchmark
            ReturnFromBenchmark(fixed);
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
        virtual Array<Line<T>> ToLines(const Matrix<T>&) const override {
            StartBenchmark
            ReturnFromBenchmark(MakeArrayFromSingle<Line<T>>(Line<T>(prevPosition, this->position)));
        }
        virtual bool CollidesWith(const Shape<T>& other) const override {
            StartBenchmark
            ReturnFromBenchmark(this->position == ((const Particle<T>&)other).position);
        }
        Matrix<T> prevPosition;

        private:
        Matrix<T> acceleration;
        bool fixed;
    };
}

#endif