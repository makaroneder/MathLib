#include "ConstantGate.hpp"

ConstantGate::ConstantGate(const MathLib::Sequence<bool>& value) : value(value) {}
MathLib::Bitmap ConstantGate::Update(const MathLib::Sequence<bool>& input) {
    return input.IsEmpty() ? value : MathLib::Bitmap();
}