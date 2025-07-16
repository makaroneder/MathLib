#ifndef MathLib_Math_SystemOfLinearEquations_H
#define MathLib_Math_SystemOfLinearEquations_H
#include "AugmentedMatrix.hpp"

namespace MathLib {
    template <typename T>
    struct SystemOfLinearEquations {
        SystemOfLinearEquations(size_t equations) : equations(AugmentedMatrix<T>(equations + 1, equations, equations)) {
            EmptyBenchmark
        }
        SystemOfLinearEquations(const AugmentedMatrix<T>& equations) : equations(equations) {
            EmptyBenchmark
        }
        [[nodiscard]] AugmentedMatrix<T> GetMatrix(void) const {
            StartAndReturnFromBenchmark(equations);
        }
        [[nodiscard]] T& EquationAt(size_t x, size_t y) {
            StartAndReturnFromBenchmark(equations.At(x, y));
        }
        [[nodiscard]] T EquationAt(size_t x, size_t y) const {
            StartAndReturnFromBenchmark(equations.At(x, y));
        }
        [[nodiscard]] T& ValueAt(size_t y) {
            StartAndReturnFromBenchmark(equations.At(equations.GetSecondOffset(), y));
        }
        [[nodiscard]] T ValueAt(size_t y) const {
            StartAndReturnFromBenchmark(equations.At(equations.GetSecondOffset(), y));
        }
        [[nodiscard]] bool AddEquation(const AugmentedMatrix<T>& equation, size_t y) {
            StartBenchmark
            const Interval<size_t> x = Interval<size_t>(0, equations.GetWidth());
            ReturnFromBenchmark(equations.Insert(equation, x, Interval<size_t>(y, y + 1), x, Interval<size_t>(0, 1)));
        }
        [[nodiscard]] bool AddEquation(const AugmentedMatrix<T>& equation) {
            StartBenchmark
            const Interval<size_t> x = Interval<size_t>(0, equations.GetWidth());
            for (size_t y = 0; y < equations.GetHeight(); y++) {
                const Interval<size_t> y1 = Interval<size_t>(y, y + 1);
                const Interval<size_t> y2 = Interval<size_t>(0, 1);
                if (equations.IsFilledWith(0, x, y1)) ReturnFromBenchmark(AddEquation(equation, y));
                if (equations.IsMultipleOf(equation, x, y1, x, y2)) ReturnFromBenchmark(true);
            }
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] Expected<Matrix<T>> GetMatrixSolution(void) const {
            StartBenchmark
            const Expected<SingleTypePair<Matrix<T>>> data = equations.GetMatrix();
            if (!data.HasValue()) ReturnFromBenchmark(Expected<Matrix<T>>());
            const Expected<Matrix<T>> tmp = data.Get().first.GetInverse();
            ReturnFromBenchmark(tmp.HasValue() ? tmp.Get() * data.Get().second : Expected<Matrix<T>>());
        }
        [[nodiscard]] Expected<Matrix<T>> GetCramerSolution(void) const {
            StartBenchmark
            const Expected<SingleTypePair<Matrix<T>>> data = equations.GetMatrix();
            if (!data.HasValue()) ReturnFromBenchmark(Expected<Matrix<T>>());
            const T det = data.Get().first.GetDeterminant();
            if (FloatsEqual<T>(det, 0)) ReturnFromBenchmark(Expected<Matrix<T>>());
            Matrix<T> ret = Matrix<T>(1, equations.GetHeight());
            const Interval<size_t> y = Interval<size_t>(0, equations.GetHeight());
            for (size_t i = 0; i < ret.GetHeight(); i++) {
                Matrix<T> tmp = data.Get().first;
                if (!tmp.Insert(data.Get().second, Interval<size_t>(i, i + 1), y, Interval<size_t>(0, 1), y)) ReturnFromBenchmark(Expected<Matrix<T>>());
                ret.At(0, i) = tmp.GetDeterminant() / det;
            }
            ReturnFromBenchmark(ret);
        }

        private:
        AugmentedMatrix<T> equations;
    };
}

#endif