#ifndef Font_H
#define Font_H
#include <stdint.h>
#include <stddef.h>

struct PSF1 {
    static constexpr uint16_t expectedMagic = 0x0436;
	uint16_t magic;
	uint8_t mode;
	uint8_t charSize;
    uint8_t buffer[];

    bool IsValid(void) const {
        return magic == expectedMagic;
    }
    size_t GetWidth(void) const {
        return 8;
    }
    size_t GetHeight(void) const {
        return charSize;
    }
    const uint8_t* GetGlyph(char chr) const {
        return &buffer[chr * charSize];
    }
} __attribute__((packed));
extern "C" uintptr_t _binary_src_Lib_zap_light16_psf_start;

#endif