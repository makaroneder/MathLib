#ifndef Printable_H
#define Printable_H
#include "Typedefs.hpp"

/// @brief Interface for printable structures
struct Printable {
    /// @brief Virtual destructor
    virtual ~Printable(void);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(String padding = "") const = 0;
};
/// @brief Converts struct to string
/// @param printable Struct to convert
/// @return String representation
String ToString(const Printable& printable);

#endif