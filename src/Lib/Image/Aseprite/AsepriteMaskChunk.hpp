#ifndef MathLib_Image_Aseprite_MaskChunk_H
#define MathLib_Image_Aseprite_MaskChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteMaskChunk : AsepriteChunkHeader {
        int16_t x;
        int16_t y;
        uint16_t width;
        uint16_t height;
        uint64_t reserved;
        AsepriteString name;
        uint8_t data[];
    } __attribute__((packed));
}

#endif