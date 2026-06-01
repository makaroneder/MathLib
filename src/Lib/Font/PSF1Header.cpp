#include "PSF1Header.hpp"

namespace MathLib {
    PSF1Header::PSF1Header(void) : signature(0), has512Glyphs(false), hasUnicodeTable(false), reserved(0), charSize(0) {}
    PSF1Header::PSF1Header(uint8_t height, bool has512Glyphs) : signature(expectedSignature), has512Glyphs(has512Glyphs), hasUnicodeTable(false), reserved(0), charSize(height) {}
    bool PSF1Header::IsValid(void) const {
        return signature == expectedSignature;
    }
    uint16_t PSF1Header::GetGlyphCount(void) const {
        return 256 + has512Glyphs * 256;
    }
}