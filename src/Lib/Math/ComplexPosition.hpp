#ifndef MathLib_Math_ComplexPosition_H
#define MathLib_Math_ComplexPosition_H
#include "Matrix.hpp"
#include "../Color.hpp"

namespace MathLib {
    template <typename T>
    struct ComplexPosition : Allocatable {
        Complex<T> position;
        Complex<T> value;

        ComplexPosition(void) {}
        ComplexPosition(const Complex<T>& pos, const Complex<T>& val) : position(pos), value(val) {}
        [[nodiscard]] Matrix<T> GetPosition(void) const {
            return CreateVector<T>(position.GetReal(), position.GetImaginary(), 0);
        }
        [[nodiscard]] uint32_t GetColor(void) const {
            return GetRainbow<T>(CreateVector<T>(value.GetReal(), value.GetImaginary(), 0).GetLength());
        }
        [[nodiscard]] bool operator==(const ComplexPosition<T>& other) const {
            return position == other.position && value == other.value;
        }
        [[nodiscard]] bool operator!=(const ComplexPosition<T>& other) const {
            return !(*this == other);
        }
    };
}

#endif