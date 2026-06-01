#ifndef MathLib_Image_Aseprite_TilesetChunk_H
#define MathLib_Image_Aseprite_TilesetChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteTilesetChunk : AsepriteChunkHeader {
        enum class Flag : uint8_t {
            LinkToExternalFile = 0,
            TilesInsideThisFile,
            ID0IsEmpty,
            AutomaticXFlipMatch,
            AutomaticYFlipMatch,
            AutomaticDiagonalFlipMatch,
        };
        uint32_t id;
        uint32_t flags;
        uint32_t tileCount;
        uint32_t width;
        uint32_t height;
        int16_t base;
        uint16_t reserved[7];
        AsepriteString name;
    } __attribute__((packed));
}

#endif