#include "Bitmap.hpp"

namespace MathLib {
    Bitmap::Bitmap(void) {}
    Bitmap::Bitmap(size_t size) : array(size / 8 + !!(size % 8)) {}
    Bitmap::Bitmap(const Array<uint8_t>& array) : array(array) {}
    size_t Bitmap::GetSize(void) const {
        return array.GetSize() * 8;
    }
    Expected<bool> Bitmap::Get(size_t i) const {
        const size_t offset = i / 8;
        return offset < array.GetSize() ? Expected<bool>(array.At(offset) & (1 << (i % 8))) : Expected<bool>();
    }
    bool Bitmap::Set(size_t i, bool value) {
        const uint8_t mask = 1 << (i % 8);
        const size_t offset = i / 8;
        if (offset >= array.GetSize()) return false;
        if (value) array.At(offset) |= mask;
        else array.At(offset) &= ~mask;
        return true;
    }
}