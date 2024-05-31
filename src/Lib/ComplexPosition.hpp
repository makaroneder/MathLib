#ifndef ComplexPosition_H
#define ComplexPosition_H
#include "Matrix.hpp"
#include "Color.hpp"

/// @brief Value of complex function
/// @tparam T Type of number
template <typename T>
struct ComplexPosition {
    /// @brief Argument of complex function
    std::complex<T> position;
    /// @brief Value of complex function
    std::complex<T> value;

    /// @brief Creates empty complex position
    ComplexPosition(void) {}
    /// @brief Creates a new complex position
    /// @param pos Argument of complex function
    /// @param val Value of complex function
    ComplexPosition(std::complex<T> pos, std::complex<T> val) : position(pos), value(val) {}
    /// @brief v = [re(p), im(p), 0]
    /// @return Argument of complex function as position
    constexpr Matrix<T> GetPosition(void) const {
        return CreateVector<T>(position.real(), position.imag(), 0);
    }
    /// @brief c = R(|[re(v), im(v), 0]|)
    /// @return Value of complex function as color
    constexpr uint32_t GetColor(void) const {
        return GetRainbow<T>(CreateVector<T>(value.real(), value.imag(), 0).GetLength());
    }
};

#endif