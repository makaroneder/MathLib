#ifndef RosslerSystem_H
#define RosslerSystem_H
#include "ChaoticSystem.hpp"
#include <Math/Vector.hpp>

template <typename T>
struct RosslerSystem  : ChaoticSystem<T> {
    RosslerSystem(const MathLib::Matrix<T>& data, const MathLib::Matrix<T>& parameters) : ChaoticSystem<T>(data, parameters) {}
    virtual void Update(const T& dt) override {
        const T x = GetX(this->data);
        const T y = GetY(this->data);
        const T z = GetZ(this->data);
        GetX(this->data) += -(y + z) * dt;
        GetY(this->data) += (x + GetX(this->parameters) * y) * dt;
        GetZ(this->data) += (GetY(this->parameters) + z * (x - GetZ(this->parameters))) * dt;
    }
};

#endif