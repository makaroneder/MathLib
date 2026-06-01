#ifndef MathLib_Image_Aseprite_PaletteChunkEntry_H
#define MathLib_Image_Aseprite_PaletteChunkEntry_H
#include <stdint.h>

namespace MathLib {
    struct AsepritePaletteChunkEntry {
        enum class Flag : uint8_t {
            HasName = 0,
        };
        uint16_t flags;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    } __attribute__((packed));
}

#endif