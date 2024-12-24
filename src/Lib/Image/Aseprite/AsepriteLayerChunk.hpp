#ifndef MathLib_Image_Aseprite_LayerChunk_H
#define MathLib_Image_Aseprite_LayerChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteLayerChunk : AsepriteChunkHeader {
        enum class Type : uint16_t {
            Normal = 0,
            Group,
            Tilemap,
        };
        enum class BlendMode : uint16_t {
            Normal = 0,
            Multiply,
            Screen,
            Overlay,
            Darken,
            Lighten,
            ColorDodge,
            ColorBurn,
            HardLight,
            SoftLight,
            Difference,
            Exclusion,
            Hue,
            Saturation,
            Color,
            Luminosity,
            Addition,
            Subtract,
            Divide,
        };
        bool visible : 1;
        bool editable : 1;
        bool lockMovement : 1;
        bool background : 1;
        bool preferLinkedCels : 1;
        bool displayCollapsed : 1;
        bool referenceLayer : 1;
        uint16_t reserved1 : 11;
        Type type;
        uint16_t childLevel;
        uint16_t defaultWidth;
        uint16_t defaultHeight;
        BlendMode blendMode;
        uint8_t opacity;
        uint8_t reserved2[3];
        AsepriteString name;
        uint32_t tilesetIndex;
    } __attribute__((packed));
}

#endif