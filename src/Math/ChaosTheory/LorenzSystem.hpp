#ifndef LorenzSystem_H
#define LorenzSystem_H
#include "ChaoticSystem.hpp"
#include <Math/Vector.hpp>

template <typename T>
struct LorenzSystem : ChaoticSystem<T> {
    LorenzSystem(const MathLib::Matrix<T>& data, const MathLib::Matrix<T>& parameters) : ChaoticSystem<T>(data, parameters) {}
    virtual void Update(const T& dt) override {
        const T x = GetX(this->data);
        const T y = GetY(this->data);
        const T z = GetZ(this->data);
        GetX(this->data) += GetX(this->parameters) * (y - x) * dt;
        GetY(this->data) += (x * (GetY(this->parameters) - z) - y) * dt;
        GetZ(this->data) += (x * y - GetZ(this->parameters) * z) * dt;
    }
};

#endif