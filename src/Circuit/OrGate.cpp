#include "OrGate.hpp"

OrGate::OrGate(const MathLib::Array<CircuitElementConnection>& children) : OnePinCircuitElement(children) {}
MathLib::Expected<bool> OrGate::EvaluateInternal(void) const {
    for (const CircuitElementConnection& child : children) {
        const MathLib::Expected<bool> tmp = child.Evaluate();
        if (!tmp.HasValue()) return MathLib::Expected<bool>();
        if (tmp.Get()) return MathLib::Expected<bool>(true);
    }
    return MathLib::Expected<bool>(false);
}
MathLib::String OrGate::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "Or: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(padding + '\t') + '\n';
    return ret + padding + '}';
}