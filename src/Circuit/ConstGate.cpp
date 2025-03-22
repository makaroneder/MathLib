#include "ConstGate.hpp"

ConstGate::ConstGate(bool value) : value(value) {}
MathLib::Expected<bool> ConstGate::EvaluateInternal(void) const {
    return MathLib::Expected<bool>(value);
}
void ConstGate::Flip(void) {
    value = !value;
}
MathLib::String ConstGate::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + (value ? "High" : "Low");
}