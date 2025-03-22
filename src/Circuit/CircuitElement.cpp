#include "CircuitElement.hpp"

MathLib::Array<CircuitElement*> elements;
CircuitElement::CircuitElement(void) {}
CircuitElement::CircuitElement(const MathLib::Sequence<CircuitElementConnection>& children) : children(MathLib::CollectionToArray<CircuitElementConnection>(children)) {}