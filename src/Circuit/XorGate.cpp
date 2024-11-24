#include "XorGate.hpp"

XorGate::XorGate(const MathLib::Array<CircuitElementConnection>& children) : OnePinCircuitElement(children) {}
MathLib::Expected<bool> XorGate::EvaluateInternal(void) const {
    bool ret = false;
    for (const CircuitElementConnection& child : children) {
        const MathLib::Expected<bool> tmp = child.Evaluate();
        if (!tmp.HasValue()) return MathLib::Expected<bool>();
        ret ^= tmp.Get();
    }
    return MathLib::Expected<bool>(ret);
}
MathLib::String XorGate::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "Or: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(padding + '\t') + '\n';
    return ret + padding + '}';
}