#ifndef MathLib_Image_Aseprite_ImageCelChunk_H
#define MathLib_Image_Aseprite_ImageCelChunk_H
#include "AsepriteCelChunk.hpp"

namespace MathLib {
    struct AsepriteImageCelChunk : AsepriteCelChunk {
        uint16_t width;
        uint16_t height;
        uint32_t pixels[];

        AsepriteImageCelChunk(uint16_t width, uint16_t height, bool raw);
    } __attribute__((packed));
}

#endif