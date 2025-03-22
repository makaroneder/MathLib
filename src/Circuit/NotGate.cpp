#include "NotGate.hpp"

NotGate::NotGate(const CircuitElementConnection& child) : OnePinCircuitElement(MathLib::MakeArray<CircuitElementConnection>(child)) {}
MathLib::Expected<bool> NotGate::EvaluateInternal(void) const {
    if (children.GetSize() != 1) return MathLib::Expected<bool>();
    const MathLib::Expected<bool> tmp = children.At(0).Evaluate();
    return tmp.HasValue() ? MathLib::Expected<bool>(!tmp.Get()) : MathLib::Expected<bool>();
}
MathLib::String NotGate::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "Not: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
    return ret + padding + '}';
}