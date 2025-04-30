#ifndef MathLib_Image_Aseprite_CelChunk_H
#define MathLib_Image_Aseprite_CelChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteCelChunk : AsepriteChunkHeader {
        enum class Type : uint16_t {
            Raw = 0,
            Linked,
            CompressedImage,
            CompressedTilemap,
        };
        uint16_t layer;
        int16_t x;
        int16_t y;
        uint8_t opacity;
        Type type;
        uint16_t z;
        uint8_t reserved[5];

        AsepriteCelChunk(uint32_t size, Type type);
    } __attribute__((packed));
}

#endif