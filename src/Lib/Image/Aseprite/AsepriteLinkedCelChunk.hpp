#ifndef MathLib_Image_Aseprite_LinkedCelChunk_H
#define MathLib_Image_Aseprite_LinkedCelChunk_H
#include "AsepriteCelChunk.hpp"

namespace MathLib {
    struct AsepriteLinkedCelChunk : AsepriteCelChunk {
        uint16_t position;
    } __attribute__((packed));
}

#endif