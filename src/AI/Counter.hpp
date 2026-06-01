#ifndef Counter_H
#define Counter_H
#include <Typedefs.hpp>

template <typename T, typename Num>
struct Counter {
    [[nodiscard]] virtual Num Count(const MathLib::Sequence<T>& text, const MathLib::Sequence<T>& document, const MathLib::Sequence<MathLib::Array<T>>& documents) const = 0;
    [[nodiscard]] MathLib::Array<Num> ToVector(const MathLib::Sequence<MathLib::Array<T>>& texts, const MathLib::Sequence<T>& document, const MathLib::Sequence<MathLib::Array<T>>& documents) const {
        const size_t size = texts.GetSize();
        MathLib::Array<Num> ret = size;
        for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = Count(texts.AtUnsafe(i), document, documents);
        return ret;
    }
};

#endif