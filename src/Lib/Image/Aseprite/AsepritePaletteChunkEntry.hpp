#ifndef MathLib_Image_Aseprite_PaletteChunkEntry_H
#define MathLib_Image_Aseprite_PaletteChunkEntry_H
#include <stdint.h>

namespace MathLib {
    struct AsepritePaletteChunkEntry {
        bool hasName : 1;
        uint16_t reserved : 15;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    } __attribute__((packed));
}

#endif