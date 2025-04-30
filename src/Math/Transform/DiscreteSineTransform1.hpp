#ifndef DiscreteSineTransform1_H
#define DiscreteSineTransform1_H
#include "DiscreteTransform.hpp"
#include <Math/Trigonometry.hpp>

template <typename T>
struct DiscreteSineTransform1 : DiscreteTransform<T> {
    [[nodiscard]] virtual MathLib::Array<T> Transform(const MathLib::Sequence<T>& input) const override {
        const size_t size = input.GetSize();
        const T multiplier = MathLib::pi / (size + 1);
        MathLib::Array<T> ret = MathLib::Array<T>(size);
        for (size_t i = 0; i < size; i++) {
            const T tmp = multiplier * (i + 1);
            ret.At(i) = T();
            for (size_t j = 0; j < size; j++) ret.At(i) += input.At(j) * MathLib::Sin(tmp * (j + 1));
        }
        return ret;
    }
    [[nodiscard]] virtual MathLib::Array<T> InverseTransform(const MathLib::Sequence<T>& input) const override {
        const size_t size = input.GetSize();
        const T multiplier = 2.0 / (size + 1);
        MathLib::Array<T> ret = Transform(input);
        for (T& x : ret) x *= multiplier;
        return ret;
    }
};

#endif