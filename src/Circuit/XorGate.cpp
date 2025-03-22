#include "XorGate.hpp"

XorGate::XorGate(const MathLib::Sequence<CircuitElementConnection>& children) : OnePinCircuitElement(children) {}
MathLib::Expected<bool> XorGate::EvaluateInternal(void) const {
    bool ret = false;
    for (const CircuitElementConnection& child : children) {
        const MathLib::Expected<bool> tmp = child.Evaluate();
        if (!tmp.HasValue()) return MathLib::Expected<bool>();
        ret ^= tmp.Get();
    }
    return MathLib::Expected<bool>(ret);
}
MathLib::String XorGate::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "Or: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
    return ret + padding + '}';
}