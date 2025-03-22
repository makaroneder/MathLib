#ifndef MathLib_Interfaces_Printable_H
#define MathLib_Interfaces_Printable_H
#include "../Typedefs.hpp"

namespace MathLib {
    /// @brief Interface for printable structures
    struct Printable : Allocatable {
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const = 0;
    };
    /// @brief Converts struct to string
    /// @param printable Struct to convert
    /// @return String representation
    [[nodiscard]] String ToString(const Printable& printable);
    [[nodiscard]] String ToString(const Sequence<char>& string);
    [[nodiscard]] String ToString(num_t x);
    template <typename T>
    String ToString(const Sequence<T>& data) {
        String ret = '{';
        const size_t size = data.GetSize();
        for (size_t i = 0; i < size; i++) ret += MathLib::ToString(data.At(i)) + (((i + 1) == size) ? "" : ", ");
        return ret + '}';
    }
}

#endif