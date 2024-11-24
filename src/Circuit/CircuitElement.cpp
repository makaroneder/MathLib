#include "CircuitElement.hpp"

MathLib::Array<CircuitElement*> elements;
CircuitElement::CircuitElement(void) {}
CircuitElement::CircuitElement(const MathLib::Array<CircuitElementConnection>& children) : children(children) {}