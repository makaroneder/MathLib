#include "Printable.hpp"

Printable::~Printable(void) {}
String ToString(const Printable& printable) {
    return printable.ToString();
}