#ifndef BagOfWordsCounter_H
#define BagOfWordsCounter_H
#include "Counter.hpp"

template <typename T, typename Num>
struct BagOfWordsCounter : Counter<T, Num> {
    [[nodiscard]] virtual Num Count(const MathLib::Sequence<T>& text, const MathLib::Sequence<T>& document, const MathLib::Sequence<MathLib::Array<T>>&) const override {
        return document.GetCountOf(text);
    }
};

#endif