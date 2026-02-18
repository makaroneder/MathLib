#include "CircuitConnection.hpp"
#include <String.hpp>

CircuitConnection::CircuitConnection(void) : element(SIZE_MAX), bit(SIZE_MAX) {}
CircuitConnection::CircuitConnection(size_t element, size_t bit) : element(element), bit(bit) {}
MathLib::String CircuitConnection::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + (element == SIZE_MAX ? "Input" : MathLib::ToString(element, 10)) + '[' + MathLib::ToString(bit, 10) + ']';
}
bool CircuitConnection::operator==(const CircuitConnection& other) const {
    return element == other.element && bit == other.bit;
}
bool CircuitConnection::operator!=(const CircuitConnection& other) const {
    return !(*this == other);
}