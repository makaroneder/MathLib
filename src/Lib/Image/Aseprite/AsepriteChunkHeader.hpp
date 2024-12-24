#ifndef MathLib_Image_Aseprite_ChunkHeader_H
#define MathLib_Image_Aseprite_ChunkHeader_H
#include <stdint.h>

namespace MathLib {
    struct AsepriteChunkHeader {
        enum class Type : uint16_t {
            OldPalette6 = 0x0004,
            OldPalette8 = 0x0011,
            Layer = 0x2004,
            Cel,
            CelExtra,
            ColorProfile,
            ExternalFiles,
            Mask = 0x2016,
            Path,
            Tags,
            Palette,
            UserData,
            Slice = 0x2022,
            Tileset,
        };
        uint32_t size;
        Type type;
    } __attribute__((packed));
}

#endif