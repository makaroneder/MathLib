#ifndef MathLib_PSF1_H
#define MathLib_PSF1_H
#include <stdint.h>
#include <stddef.h>

namespace MathLib {
    /// @brief PSF1 font
    struct PSF1 {
        /// @brief Expected value of signature
        static constexpr uint16_t expectedSignature = 0x0436;
        /// @brief Signature of the font
        uint16_t signature;
        bool has512Glyphs : 1;
        bool hasUnicodeTable : 2;
        uint8_t reserved : 5;
        /// @brief Size of characters in the font
        uint8_t charSize;
        /// @brief Glyph buffer
        uint8_t buffer[];

        /// @brief Checks whether the font is valid
        /// @return Validation status
        [[nodiscard]] bool IsValid(void) const;
        /// @brief Returns width of each glyph in the font
        /// @return Width of each glyph in the font
        [[nodiscard]] size_t GetWidth(void) const;
        /// @brief Returns height of each glyph in the font
        /// @return Height of each glyph in the font
        [[nodiscard]] size_t GetHeight(void) const;
        /// @brief Returns glyph
        /// @param chr Character to return glyph of
        /// @return Glyph
        [[nodiscard]] const uint8_t* GetGlyph(char chr) const;
    } __attribute__((packed));
}

#endif