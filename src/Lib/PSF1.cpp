#include "PSF1.hpp"

namespace MathLib {
    bool PSF1::IsValid(void) const {
        return signature == expectedSignature;
    }
    size_t PSF1::GetWidth(void) const {
        return 8;
    }
    size_t PSF1::GetHeight(void) const {
        return charSize;
    }
    const uint8_t* PSF1::GetGlyph(char chr) const {
        return &buffer[chr * charSize];
    }
}