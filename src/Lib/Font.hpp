#ifndef Font_H
#define Font_H
#include <stdint.h>
#include <stddef.h>

/// @brief PSF1 font
struct PSF1 {
    /// @brief Expected value of signature
    static constexpr uint16_t expectedSignature = 0x0436;
	/// @brief Signature of the font
	uint16_t signature;
	/// @brief Mode of the font
	uint8_t mode;
	/// @brief Size of characters in the font
	uint8_t charSize;
    /// @brief Glyph buffer
    uint8_t buffer[];

    /// @brief Checks whether the font is valid
    /// @return Validation status
    bool IsValid(void) const {
        return signature == expectedSignature;
    }
    /// @brief Returns width of each glyph in the font
    /// @return Width of each glyph in the font
    size_t GetWidth(void) const {
        return 8;
    }
    /// @brief Returns height of each glyph in the font
    /// @return Height of each glyph in the font
    size_t GetHeight(void) const {
        return charSize;
    }
    /// @brief Returns glyph
    /// @param chr Character to return glyph of
    /// @return Glyph
    const uint8_t* GetGlyph(const char& chr) const {
        return &buffer[chr * charSize];
    }
} __attribute__((packed));

/// @brief src/Lib/zap-light16.psf
extern "C" PSF1 _binary_src_Lib_zap_light16_psf_start;

#endif