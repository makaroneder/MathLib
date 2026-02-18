#ifndef MathLib_Image_Aseprite_TilesetChunk_H
#define MathLib_Image_Aseprite_TilesetChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteTilesetChunk : AsepriteChunkHeader {
        uint32_t id;
        bool linkToExternalFile : 1;
        bool tilesInsideThisFile : 1;
        bool id0IsEmpty : 1;
        bool automaticXFlipMatch : 1;
        bool automaticYFlipMatch : 1;
        bool automaticDiagonalFlipMatch : 1;
        uint32_t reserved : 26;
        uint32_t tileCount;
        uint32_t width;
        uint32_t height;
        int16_t base;
        uint16_t reserved[7];
        AsepriteString name;
    } __attribute__((packed));
}

#endif