#include "NonImplicationGate.hpp"

MathLib::Bitmap NonImplicationGate::Update(const MathLib::Sequence<bool>& input) {
    return input.GetSize() == 2 ? MathLib::MakeBitmap(input.At(0) && !input.At(1)) : MathLib::Bitmap();
}