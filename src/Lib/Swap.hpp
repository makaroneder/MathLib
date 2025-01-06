#ifndef MathLib_Swap_H
#define MathLib_Swap_H

namespace MathLib {
    /// @brief Swaps 2 values
    /// @tparam T Type of value
    /// @param a First value to swap
    /// @param b Second value to swap
    template <typename T>
    void Swap(T& a, T& b) {
        const T tmp = a;
        a = b;
        b = tmp;
    }
}

#endif
