#include "CircuitElementConnection.hpp"
#include "CircuitElement.hpp"

CircuitElementConnection::CircuitElementConnection(void) {}
CircuitElementConnection::CircuitElementConnection(size_t index, size_t pin) : index(index), pin(pin) {}
MathLib::Expected<bool> CircuitElementConnection::Evaluate(void) const {
    const MathLib::Expected<MathLib::Bitmap> ret = elements.At(index)->Evaluate();
    if (!ret.HasValue()) return MathLib::Expected<bool>();
    return ret.Get().Get(pin);
}
MathLib::String CircuitElementConnection::ToString(const MathLib::Sequence<char>& padding) const {
    return elements.At(index)->ToString(padding) + " (" + MathLib::ToString(pin) + ')';
}