#include "NotOrGate.hpp"

MathLib::Bitmap NotOrGate::Update(const MathLib::Sequence<bool>& input) {
    const size_t size = input.GetSize();
    for (size_t i = 0; i < size; i++)
        if (input.At(i)) return MathLib::MakeBitmap(false);
    return MathLib::MakeBitmap(true);
}