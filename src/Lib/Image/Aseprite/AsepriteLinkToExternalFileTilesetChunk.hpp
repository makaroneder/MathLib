#ifndef MathLib_Image_Aseprite_LinkToExternalFileTilesetChunk_H
#define MathLib_Image_Aseprite_LinkToExternalFileTilesetChunk_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteLinkToExternalFileTilesetChunk {
        uint32_t externalFileID;
        uint32_t tilesetID;
    } __attribute__((packed));
}

#endif