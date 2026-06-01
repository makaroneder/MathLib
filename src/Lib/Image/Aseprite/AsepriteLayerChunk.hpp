#ifndef MathLib_Image_Aseprite_LayerChunk_H
#define MathLib_Image_Aseprite_LayerChunk_H
#include "AsepriteChunkHeader.hpp"
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteLayerChunk : AsepriteChunkHeader {
        enum class Flag : uint8_t {
            Visible = 0,
            Editable,
            LockMovement,
            Background,
            PreferLinkedCels,
            DisplayCollapsed,
            ReferenceLayer,
        };
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
        uint16_t flags;
        Type type;
        uint16_t childLevel;
        uint16_t defaultWidth;
        uint16_t defaultHeight;
        BlendMode blendMode;
        uint8_t opacity;
        uint8_t reserved2[3];
        AsepriteString name;

        AsepriteLayerChunk(uint16_t width, uint16_t height);
    } __attribute__((packed));
}

#endif