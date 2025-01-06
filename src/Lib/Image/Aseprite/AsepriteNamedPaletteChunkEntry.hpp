#ifndef MathLib_Image_Aseprite_NamedPaletteChunkEntry_H
#define MathLib_Image_Aseprite_NamedPaletteChunkEntry_H
#include "AsepritePaletteChunkEntry.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteNamedPaletteChunkEntry : AsepritePaletteChunkEntry {
        AsepriteString name;
    } __attribute__((packed));
}

#endif