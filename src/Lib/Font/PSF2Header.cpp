#include "PSF2Header.hpp"

namespace MathLib {
    PSF2Header::PSF2Header(void) : signature(0), version(0), headerSize(0), hasUnicodeTable(false), reserved(0), glyphCount(0), bytesPerGlyph(0), height(0), width(0) {}
    PSF2Header::PSF2Header(uint32_t width, uint32_t height, uint32_t glyphCount, uint32_t bytesPerGlyph) : signature(expectedSignature), version(expectedVersion), headerSize(sizeof(PSF2Header)), hasUnicodeTable(false), reserved(0), glyphCount(glyphCount), bytesPerGlyph(bytesPerGlyph), height(height), width(width) {}
    bool PSF2Header::IsValid(void) const {
        return signature == expectedSignature && version == expectedVersion && headerSize >= sizeof(PSF2Header);
    }
}