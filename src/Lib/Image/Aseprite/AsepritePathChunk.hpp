#ifndef MathLib_Image_Aseprite_PathChunk_H
#define MathLib_Image_Aseprite_PathChunk_H
#include "AsepriteChunkHeader.hpp"

namespace MathLib {
    struct AsepritePathChunk : AsepriteChunkHeader {} __attribute__((packed));
}

#endif