#ifndef MathLib_Image_Aseprite_Header_H
#define MathLib_Image_Aseprite_Header_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteHeader {
        static constexpr uint16_t expectedSignature = 0xa5e0;
        uint32_t size;
        uint16_t signature;
        uint16_t frames;
        uint16_t width;
        uint16_t height;
        uint16_t bpp;
        bool hasOpacityLayer : 1;
        uint32_t reserved1 : 31;
        uint16_t speed;
        uint64_t reserved2;
        uint8_t paletteEntry;
        uint8_t reserved3[3];
        uint16_t colors;
        uint8_t pixelWidth;
        uint8_t pixelHeight;
        int16_t gridX;
        int16_t gridY;
        uint16_t gridWidth;
        uint16_t gridHeight;
        uint32_t reserved4[21];

        AsepriteHeader(void);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif