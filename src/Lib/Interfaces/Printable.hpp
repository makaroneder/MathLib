#ifndef MathLib_Interfaces_Printable_H
#define MathLib_Interfaces_Printable_H
#include "Formatter.hpp"

namespace MathLib {
    /// @brief Interface for printable structures
    struct Printable : Allocatable {
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const = 0;
    };
    MakeFormatter(Printable, self, padding, {
        return self.ToString(padding);
    });
    /// @brief Converts struct to string
    /// @param printable Struct to convert
    /// @return String representation
    [[nodiscard]] String ToString(const Printable& printable);
    [[nodiscard]] String ToString(const Sequence<char>& string);
    template <typename T>
    String ToString(const Sequence<T>& data) {
        String ret = '{';
        const size_t size = data.GetSize();
        for (size_t i = 0; i < size; i++) ret += Formatter<T>::ToString(data.At(i)) + (((i + 1) == size) ? "" : ", ");
        return ret + '}';
    }
}

#endif