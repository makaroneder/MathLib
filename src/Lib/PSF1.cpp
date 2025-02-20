#include "PSF1.hpp"
#include "Typedefs.hpp"

namespace MathLib {
    bool PSF1::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
    size_t PSF1::GetWidth(void) const {
        StartAndReturnFromBenchmark(8);
    }
    size_t PSF1::GetHeight(void) const {
        StartAndReturnFromBenchmark(charSize);
    }
    const uint8_t* PSF1::GetGlyph(char chr) const {
        StartAndReturnFromBenchmark(&buffer[chr * charSize]);
    }
}