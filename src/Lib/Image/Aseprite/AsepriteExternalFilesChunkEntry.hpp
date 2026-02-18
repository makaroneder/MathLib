#ifndef MathLib_Image_Aseprite_ExternalFilesChunkEntry_H
#define MathLib_Image_Aseprite_ExternalFilesChunkEntry_H
#include "AsepriteString.hpp"

namespace MathLib {
    struct AsepriteExternalFilesChunkEntry {
        enum class Type : uint8_t {
            ExternalPalette = 0,
            ExternalTileset,
            PropertiesExtensionName,
            TileManagementExtensionName,
        };
        uint32_t id;
        Type type;
        uint8_t reserved[7];
        AsepriteString path;
    } __attribute__((packed));
}

#endif