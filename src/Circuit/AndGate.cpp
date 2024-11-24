#include "AndGate.hpp"

AndGate::AndGate(const MathLib::Array<CircuitElementConnection>& children) : OnePinCircuitElement(children) {}
MathLib::Expected<bool> AndGate::EvaluateInternal(void) const {
    for (const CircuitElementConnection& child : children) {
        const MathLib::Expected<bool> tmp = child.Evaluate();
        if (!tmp.HasValue()) return MathLib::Expected<bool>();
        if (!tmp.Get()) return MathLib::Expected<bool>(false);
    }
    return MathLib::Expected<bool>(true);
}
MathLib::String AndGate::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "And: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(padding + '\t') + '\n';
    return ret + padding + '}';
}