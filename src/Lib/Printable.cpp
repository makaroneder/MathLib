#include "Printable.hpp"

Printable::~Printable(void) {}
std::ostream& operator<<(std::ostream& stream, const Printable& printable) {
    return stream << printable.ToString();
}