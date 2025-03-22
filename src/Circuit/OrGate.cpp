#include "OrGate.hpp"

OrGate::OrGate(const MathLib::Sequence<CircuitElementConnection>& children) : OnePinCircuitElement(children) {}
MathLib::Expected<bool> OrGate::EvaluateInternal(void) const {
    for (const CircuitElementConnection& child : children) {
        const MathLib::Expected<bool> tmp = child.Evaluate();
        if (!tmp.HasValue()) return MathLib::Expected<bool>();
        if (tmp.Get()) return MathLib::Expected<bool>(true);
    }
    return MathLib::Expected<bool>(false);
}
MathLib::String OrGate::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "Or: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
    return ret + padding + '}';
}