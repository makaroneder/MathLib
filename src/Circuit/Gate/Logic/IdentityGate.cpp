#include "IdentityGate.hpp"

MathLib::Bitmap IdentityGate::Update(const MathLib::Sequence<bool>& input) {
    return MathLib::Bitmap(input);
}