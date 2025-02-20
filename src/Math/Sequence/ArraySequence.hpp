#ifndef ArraySequence_H
#define ArraySequence_H
#include "Sequence.hpp"
#include <Typedefs.hpp>

template <typename T>
struct ArraySequence : Sequence<T, size_t> {
    ArraySequence(const MathLib::Array<T>& data) : data(data) {}
    [[nodiscard]] bool Add(const T& x) const {
        return data.Add(x);
    }
    [[nodiscard]] virtual size_t GetSize(void) const override {
        return data.GetSize();
    }
    [[nodiscard]] virtual T At(const size_t& index) const override {
        return data.At(index);
    }

    private:
    MathLib::Array<T> data;
};

#endif