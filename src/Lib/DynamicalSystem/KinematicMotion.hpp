#ifndef MathLib_DynamicalSystem_KinematicMotion_H
#define MathLib_DynamicalSystem_KinematicMotion_H
#include "DynamicalSystem.hpp"

namespace MathLib {
    template <typename T>
    struct KinematicMotion : DynamicalSystem<T> {
        KinematicMotion(const Sequence<Matrix<T>>& data) : data(CollectionToArray<Matrix<T>>(data)) {    }
        virtual Matrix<T> Update(const T& dt) override {
            data.At(data.GetSize() - 1) = GetFinal();
            for (size_t i = data.GetSize() - 1; i; i--) data.At(i - 1) += data.At(i) * dt;
            return data.At(0);
        }

        protected:
        [[nodiscard]] virtual Matrix<T> GetFinal(void) = 0;

        private:
        Array<Matrix<T>> data;
    };
}

#endif