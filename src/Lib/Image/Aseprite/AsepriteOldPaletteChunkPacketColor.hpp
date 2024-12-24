#ifndef MathLib_Image_Aseprite_OldPaletteChunkPacketColor_H
#define MathLib_Image_Aseprite_OldPaletteChunkPacketColor_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteOldPaletteChunkPacketColor {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } __attribute__((packed));
}

#endif