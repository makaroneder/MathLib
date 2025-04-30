#ifndef MathLib_Math_AugmentedMatrix_H
#define MathLib_Math_AugmentedMatrix_H
#include "../SingleTypePair.hpp"
#include "Matrix.hpp"

namespace MathLib {
    /// @brief Structure representing mathematic matrixes
    /// @tparam T Type of number
    template <typename T>
    struct AugmentedMatrix : Matrix<T> {
        AugmentedMatrix(size_t width, size_t height, size_t secondStart) : Matrix<T>(width, height), secondStart(secondStart) {
            EmptyBenchmark
        }
        AugmentedMatrix(size_t width, size_t height, const Sequence<T>& data, size_t secondStart) : Matrix<T>(width, height, data), secondStart(secondStart) {
            EmptyBenchmark
        }
        AugmentedMatrix(const Matrix<T>& a, const Matrix<T>& b) : Matrix<T>(a.GetWidth() + b.GetWidth(), a.GetHeight()), secondStart(a.GetWidth()) {
            StartBenchmark
            for (size_t y = 0; y < a.GetHeight(); y++) {
                for (size_t x = 0; x < a.GetWidth(); x++) this->At(x, y) = a.At(x, y);
                for (size_t x = 0; x < b.GetWidth(); x++) this->At(x + a.GetWidth(), y) = b.At(x, y);
            }
            EndBenchmark
        }
        AugmentedMatrix(const Matrix<T>& data, size_t secondStart) : Matrix<T>(data), secondStart(secondStart) {
            EmptyBenchmark
        }
        [[nodiscard]] size_t GetSecondOffset(void) const {
            return secondStart;
        }
        [[nodiscard]] Expected<SingleTypePair<Matrix<T>>> GetMatrix(void) const {
            StartBenchmark
            Matrix<T> a = Matrix<T>(secondStart, this->GetHeight());
            Matrix<T> b = Matrix<T>(this->GetWidth() - secondStart, this->GetHeight());
            const Interval<size_t> y = Interval<size_t>(0, a.GetHeight());
            if (!a.Insert(*this, Interval<size_t>(0, a.GetWidth()), y, Interval<size_t>(0, a.GetWidth()), y)) ReturnFromBenchmark(Expected<SingleTypePair<Matrix<T>>>());
            if (!b.Insert(*this, Interval<size_t>(0, b.GetWidth()), y, Interval<size_t>(a.GetWidth(), a.GetWidth() + b.GetWidth()), y)) ReturnFromBenchmark(Expected<SingleTypePair<Matrix<T>>>());
            ReturnFromBenchmark(SingleTypePair<Matrix<T>>(a, b));
        }

        private:
        size_t secondStart;
    };
}

#endif