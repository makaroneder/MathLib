#ifndef ChaoticSystem_H
#define ChaoticSystem_H
#include <Math/Matrix.hpp>

template <typename T>
struct ChaoticSystem {
    ChaoticSystem(const MathLib::Matrix<T>& data, const MathLib::Matrix<T>& parameters) : data(data), parameters(parameters) {}
    MathLib::Matrix<T> GetData(void) const {
        return data;
    }
    virtual void Update(const T& dt) = 0;

    protected:
    MathLib::Matrix<T> data;
    MathLib::Matrix<T> parameters;
};

#endif