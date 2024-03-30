#ifndef Printable_H
#define Printable_H
#include <ostream>

/// @brief Interface for printable structures
struct Printable {
    /// @brief Virtual destructor
    virtual ~Printable(void);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual std::string ToString(std::string padding = "") const = 0;
};
/// @brief Prints a string representation of printable structures
/// @param stream Stream to print data into
/// @param printable Data to print
/// @return Stream
std::ostream& operator<<(std::ostream& stream, const Printable& printable);

#endif