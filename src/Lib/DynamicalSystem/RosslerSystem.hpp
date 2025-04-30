#ifndef MathLib_DynamicalSystem_RosslerSystem_H
#define MathLib_DynamicalSystem_RosslerSystem_H
#include "DynamicalSystem.hpp"
#include "../Math/Vector.hpp"

namespace MathLib {
    template <typename T>
    struct RosslerSystem  : DynamicalSystem<T> {
        RosslerSystem(const Matrix<T>& data, const T& a, const T& b, const T&c) : data(data), a(a), b(b), c(c) {
            EmptyBenchmark
        }
        virtual void Update(const T& dt) override {
            StartBenchmark
            const T x = GetX(data);
            const T y = GetY(data);
            const T z = GetZ(data);
            GetX(data) += -(y + z) * dt;
            GetY(data) += (x + a * y) * dt;
            GetZ(data) += (b + z * (x - c)) * dt;
            ReturnFromBenchmark(data);
        }

        private:
        Matrix<T> data;
        T a;
        T b;
        T c;
    };
}

#endif