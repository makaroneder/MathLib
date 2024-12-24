#include "PSF1.hpp"
#include "Typedefs.hpp"

namespace MathLib {
    bool PSF1::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(signature == expectedSignature);
    }
    size_t PSF1::GetWidth(void) const {
        StartBenchmark
        ReturnFromBenchmark(8);
    }
    size_t PSF1::GetHeight(void) const {
        StartBenchmark
        ReturnFromBenchmark(charSize);
    }
    const uint8_t* PSF1::GetGlyph(char chr) const {
        StartBenchmark
        ReturnFromBenchmark(&buffer[chr * charSize]);
    }
}