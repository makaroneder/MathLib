#ifndef MathLib_Image_Aseprite_ColorProfileChunk_H
#define MathLib_Image_Aseprite_ColorProfileChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepriteColorProfileChunk : AsepriteChunkHeader {
        enum class Type : uint16_t {
            NoColorProfile = 0,
            SRGB,
            ICC,
        } type;
        bool useSpecialGamma : 1;
        uint32_t reserved1 : 31;
        uint32_t gamma;
        uint64_t reserved2;
    } __attribute__((packed));
}

#endif