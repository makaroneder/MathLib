#ifndef MathLib_Math_AugmentedMatrix_H
#define MathLib_Math_AugmentedMatrix_H
#include "../SingleTypePair.hpp"
#include "Matrix.hpp"

namespace MathLib {
    /// @brief Structure representing mathematic matrixes
    /// @tparam T Type of number
    template <typename T>
    struct AugmentedMatrix : Matrix<T> {
        AugmentedMatrix(size_t width, size_t height, size_t secondStart) : Matrix<T>(width, height), secondStart(secondStart) {}
        AugmentedMatrix(size_t width, size_t height, const Sequence<T>& data, size_t secondStart) : Matrix<T>(width, height, data), secondStart(secondStart) {}
        AugmentedMatrix(const Matrix<T>& a, const Matrix<T>& b) : Matrix<T>(a.GetWidth() + b.GetWidth(), a.GetHeight()), secondStart(a.GetWidth()) {
            for (size_t y = 0; y < a.GetHeight(); y++) {
                for (size_t x = 0; x < a.GetWidth(); x++) this->At(x, y) = a.At(x, y);
                for (size_t x = 0; x < b.GetWidth(); x++) this->At(x + a.GetWidth(), y) = b.At(x, y);
            }
        }
        AugmentedMatrix(const Matrix<T>& data, size_t secondStart) : Matrix<T>(data), secondStart(secondStart) {}
        [[nodiscard]] size_t GetSecondOffset(void) const {
            return secondStart;
        }
        [[nodiscard]] Expected<SingleTypePair<Matrix<T>>> GetMatrix(void) const {
            Matrix<T> a = Matrix<T>(secondStart, this->GetHeight());
            Matrix<T> b = Matrix<T>(this->GetWidth() - secondStart, this->GetHeight());
            const Interval<size_t> y = Interval<size_t>(0, a.GetHeight());
            if (!a.Insert(*this, Interval<size_t>(0, a.GetWidth()), y, Interval<size_t>(0, a.GetWidth()), y)) return Expected<SingleTypePair<Matrix<T>>>();
            if (!b.Insert(*this, Interval<size_t>(0, b.GetWidth()), y, Interval<size_t>(a.GetWidth(), a.GetWidth() + b.GetWidth()), y)) return Expected<SingleTypePair<Matrix<T>>>();
            return SingleTypePair<Matrix<T>>(a, b);
        }

        private:
        size_t secondStart;
    };
}

#endif