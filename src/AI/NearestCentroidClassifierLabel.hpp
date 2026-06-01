#ifndef NearestCentroidClassifierLabel_H
#define NearestCentroidClassifierLabel_H
#include <Typedefs.hpp>

template <typename T>
struct NearestCentroidClassifierLabel;
template <typename T>
struct NearestCentroidClassifierLabel : MathLib::Comparable<NearestCentroidClassifierLabel<T>> {
    NearestCentroidClassifierLabel(void) : sum(), elements(0) {}
    NearestCentroidClassifierLabel(const MathLib::Array<T>& data) : sum(data), elements(1) {}
    NearestCentroidClassifierLabel(const MathLib::Sequence<T>& data) : sum(MathLib::CollectionToArray<T>(data)), elements(1) {}
    NearestCentroidClassifierLabel(const MathLib::Array<T>& sum, size_t elements) : sum(sum), elements(elements) {}
    [[nodiscard]] T GetDistanceSquared(const MathLib::Sequence<T>& data) const {
        const size_t size = sum.GetSize();
        if (data.GetSize() != size) return 0;
        T ret = 0;
        for (size_t i = 0; i < size; i++) {
            const T tmp = sum.AtUnsafe(i) / elements - data.AtUnsafe(i);
            ret += tmp * tmp;
        }
        return ret;
    }
    [[nodiscard]] bool Add(const MathLib::Sequence<T>& data) {
        const size_t size = sum.GetSize();
        if (data.GetSize() != size) return false;
        for (size_t i = 0; i < size; i++) sum.AtUnsafe(i) += data.AtUnsafe(i);
        ++elements;
        return true;
    }

    protected:
    [[nodiscard]] virtual bool Equals(const NearestCentroidClassifierLabel<T>& other) const override {
        return elements == other.elements && sum == other.sum;
    }

    private:
    MathLib::Array<T> sum;
    size_t elements;
};

#endif