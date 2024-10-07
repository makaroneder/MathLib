#ifndef Math_ComplexPosition_H
#define Math_ComplexPosition_H
#include "Matrix.hpp"
#include "../Color.hpp"

/// @brief Value of complex function
/// @tparam T Type of number
template <typename T>
struct ComplexPosition : Allocatable {
    /// @brief Argument of complex function
    Complex<T> position;
    /// @brief Value of complex function
    Complex<T> value;

    /// @brief Creates empty complex position
    ComplexPosition(void) {}
    /// @brief Creates a new complex position
    /// @param pos Argument of complex function
    /// @param val Value of complex function
    ComplexPosition(const Complex<T>& pos, const Complex<T>& val) : position(pos), value(val) {}
    /// @brief v = [re(p), im(p), 0]
    /// @return Argument of complex function as position
    constexpr Matrix<T> GetPosition(void) const {
        return CreateVector<T>(position.GetReal(), position.GetImaginary(), 0);
    }
    /// @brief c = R(|[re(v), im(v), 0]|)
    /// @return Value of complex function as color
    constexpr uint32_t GetColor(void) const {
        return GetRainbow<T>(CreateVector<T>(value.GetReal(), value.GetImaginary(), 0).GetLength());
    }
};

#endif