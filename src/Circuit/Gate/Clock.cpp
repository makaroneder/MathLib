#include "Clock.hpp"

Clock::Clock(bool value) : value(value) {}
MathLib::Bitmap Clock::Update(const MathLib::Sequence<bool>& input) {
    if (!input.IsEmpty()) return MathLib::Bitmap();
    const bool ret = value;
    value = !value;
    return MathLib::MakeBitmap(ret);
}