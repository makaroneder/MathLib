#ifndef MathLib_Image_Aseprite_TilesInsideThisFileTilesetChunk_H
#define MathLib_Image_Aseprite_TilesInsideThisFileTilesetChunk_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteTilesInsideThisFileTilesetChunk {
        uint32_t size;
        uint32_t tileset[];
    } __attribute__((packed));
}

#endif