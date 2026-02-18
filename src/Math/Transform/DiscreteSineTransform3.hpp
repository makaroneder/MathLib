#ifndef DiscreteSineTransform3_H
#define DiscreteSineTransform3_H
#include "DiscreteTransform.hpp"
#include <Math/Trigonometry.hpp>

template <typename T>
struct DiscreteSineTransform2;
template <typename T>
struct DiscreteSineTransform3 : DiscreteTransform<T> {
    [[nodiscard]] virtual MathLib::Array<T> Transform(const MathLib::Sequence<T>& input) const override {
        const size_t size = input.GetSize();
        const T multiplier = MathLib::pi / size;
        MathLib::Array<T> ret = MathLib::Array<T>(size);
        for (size_t i = 0; i < size; i++) {
            const T tmp = multiplier * (i + 0.5);
            ret.At(i) = MathLib::Pow(-1, i) * input.At(size - 1) / 2;
            for (size_t j = 0; j < size - 1; j++) ret.At(i) += input.At(j) * MathLib::Sin(tmp * (j + 1));
        }
        return ret;
    }
    [[nodiscard]] virtual MathLib::Array<T> InverseTransform(const MathLib::Sequence<T>& input) const override {
        const size_t size = input.GetSize();
        const T multiplier = 2.0 / size;
        MathLib::Array<T> ret = DiscreteSineTransform2<T>().Transform(input);
        for (T& x : ret) x *= multiplier;
        return ret;
    }
};

#endif