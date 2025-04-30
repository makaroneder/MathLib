#ifndef DiscreteTransform_H
#define DiscreteTransform_H
#include <Typedefs.hpp>

template <typename T>
struct DiscreteTransform {
    [[nodiscard]] virtual MathLib::Array<T> Transform(const MathLib::Sequence<T>& input) const = 0;
    [[nodiscard]] virtual MathLib::Array<T> InverseTransform(const MathLib::Sequence<T>& input) const = 0;
    MathLib::Array<T> CheckTransformation(const MathLib::Sequence<T>& input) const {
        return InverseTransform(Transform(input));
    }
    bool TestTransformation(const MathLib::Sequence<T>& input) const {
        const size_t size = input.GetSize();
        const MathLib::Array<T> tmp = CheckTransformation(input);
        if (size != tmp.GetSize()) return false;
        for (size_t i = 0; i < size; i++)
            if (!MathLib::FloatsEqual<T>(tmp.At(i), input.At(i))) return false;
        return true;
    }
};

#endif