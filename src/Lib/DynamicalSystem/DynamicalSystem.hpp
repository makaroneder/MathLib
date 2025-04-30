#ifndef MathLib_DynamicalSystem_System_H
#define MathLib_DynamicalSystem_System_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    template <typename T>
    struct DynamicalSystem {
        virtual Matrix<T> Update(const T& dt) = 0;
    };
}

#endif