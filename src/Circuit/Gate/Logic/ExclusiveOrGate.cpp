#include "ExclusiveOrGate.hpp"

MathLib::Bitmap ExclusiveOrGate::Update(const MathLib::Sequence<bool>& input) {
    const size_t size = input.GetSize();
    bool ret = false;
    for (size_t i = 0; i < size; i++) ret ^= input.At(i);
    return MathLib::MakeBitmap(ret);
}