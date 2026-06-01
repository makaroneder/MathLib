#ifndef MathLib_Font_PSF2Header_H
#define MathLib_Font_PSF2Header_H
#include <stdint.h>

namespace MathLib {
    struct PSF2Header {
        static constexpr uint32_t expectedSignature = 0x864ab572;
        static constexpr uint32_t expectedVersion = 0;
        uint32_t signature;
        uint32_t version;
        uint32_t headerSize;
        bool hasUnicodeTable : 1;
        uint32_t reserved : 31;
        uint32_t glyphCount;
        uint32_t bytesPerGlyph;
        uint32_t height;
        uint32_t width;

        PSF2Header(void);
        PSF2Header(uint32_t width, uint32_t height, uint32_t glyphCount, uint32_t bytesPerGlyph);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif