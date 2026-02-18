#ifndef MathLib_DynamicalSystem_LorenzSystem_H
#define MathLib_DynamicalSystem_LorenzSystem_H
#include "DynamicalSystem.hpp"
#include "../Math/Vector.hpp"

namespace MathLib {
    template <typename T>
    struct LorenzSystem : DynamicalSystem<T> {
        LorenzSystem(const Matrix<T>& data, const T& s, const T& p, const T& b) : data(data), s(s), p(p), b(b) {}
        virtual Matrix<T> Update(const T& dt) override {
            const T x = GetX(data);
            const T y = GetY(data);
            const T z = GetZ(data);
            GetX(data) += s * (y - x) * dt;
            GetY(data) += (x * (p - z) - y) * dt;
            GetZ(data) += (x * y - b * z) * dt;
            return data;
        }

        private:
        Matrix<T> data;
        T s;
        T p;
        T b;
    };
}

#endif