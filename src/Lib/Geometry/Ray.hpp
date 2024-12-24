#ifndef MathLib_Geometry_Ray_H
#define MathLib_Geometry_Ray_H
#include "LineShape.hpp"
#include "../Math/Quaternion.hpp"

namespace MathLib {
    template <typename T>
    struct Ray : LineShape<T> {
        Ray(Matrix<T> position, Matrix<T> direction) : LineShape<T>(position), direction(direction.Normalize()) {
            EmptyBenchmark
        }
        Ray(Line<T> line) : LineShape<T>(line.start), direction((line.end - line.start).Normalize()) {
            EmptyBenchmark
        }
        Matrix<T> At(const T& t) const {
            StartBenchmark
            ReturnFromBenchmark(this->position + direction * t);
        }
        Matrix<T> GetDirection(void) const {
            return direction;
        }
        virtual bool CollidesWith(const Shape<T>&) const override {
            // TODO:
            StartBenchmark
            ReturnFromBenchmark(false);
        }
        virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
            StartBenchmark
            ReturnFromBenchmark(MakeArrayFromSingle<Line<T>>(Line<T>(this->position, RotateVector<T>(At(1), this->position, rotation))));
        }

        private:
        Matrix<T> direction;
    };
}

#endif