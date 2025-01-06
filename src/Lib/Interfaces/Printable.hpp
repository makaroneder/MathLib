#ifndef MathLib_Interfaces_Printable_H
#define MathLib_Interfaces_Printable_H
#include "../Typedefs.hpp"

namespace MathLib {
    /// @brief Interface for printable structures
    struct Printable : Allocatable {
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const String& padding = "") const = 0;
    };
    /// @brief Converts struct to string
    /// @param printable Struct to convert
    /// @return String representation
    [[nodiscard]] String ToString(const Printable& printable);
    [[nodiscard]] String ToString(const String& string);
}

#endif