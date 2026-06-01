#ifndef MathLib_Font_PSF1Header_H
#define MathLib_Font_PSF1Header_H
#include <stdint.h>

namespace MathLib {
    struct PSF1Header {
        static constexpr uint16_t expectedSignature = 0x0436;
        uint16_t signature;
        bool has512Glyphs : 1;
        bool hasUnicodeTable : 2;
        uint8_t reserved : 5;
        uint8_t charSize;

        PSF1Header(void);
        PSF1Header(uint8_t height, bool has512Glyphs);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] uint16_t GetGlyphCount(void) const;
    } __attribute__((packed));
}

#endif