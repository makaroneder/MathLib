#ifndef TfIdf_H
#define TfIdf_H
#include "Counter.hpp"
#include <Math/Log.hpp>

template <typename T, typename Num>
struct TfIdf : Counter<T, Num> {
    [[nodiscard]] virtual Num Count(const MathLib::Sequence<T>& text, const MathLib::Sequence<T>& document, const MathLib::Sequence<MathLib::Array<T>>& documents) const override {
        const Num freq = (Num)document.GetCountOf(text) / document.GetCountOfSubSequences(text.GetSize());
        const size_t documentsSize = documents.GetSize();
        size_t count = 0;
        for (size_t i = 0; i < documentsSize; i++) count += documents.AtUnsafe(i).Contains(text);
        return freq * MathLib::Log<Num>(10, (Num)documentsSize / count);
    }
};

#endif