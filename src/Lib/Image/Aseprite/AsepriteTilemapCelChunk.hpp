#ifndef MathLib_Image_Aseprite_TilemapCelChunk_H
#define MathLib_Image_Aseprite_TilemapCelChunk_H
#include "AsepriteCelChunk.hpp"

namespace MathLib {
    struct AsepriteTilemapCelChunk : AsepriteCelChunk {
        uint16_t width;
        uint16_t height;
        uint16_t bitsPerTile;
        uint32_t idBitmask;
        uint32_t xFlipBitmask;
        uint32_t yFlipBitmask;
        uint32_t diagonalFlipBitmask;
        uint16_t reserved[5];
        uint8_t tiles[];
    } __attribute__((packed));
}

#endif