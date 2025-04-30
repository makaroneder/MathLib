#include "NotGate.hpp"

MathLib::Bitmap NotGate::Update(const MathLib::Sequence<bool>& input) {
    const size_t size = input.GetSize();
    MathLib::Bitmap ret = MathLib::Bitmap(size);
    for (size_t i = 0; i < size; i++)
        if (!ret.Set(i, !input.At(i))) return MathLib::Bitmap();
    return ret;
}