#ifndef MathLib_Image_PNG_Chunk_H
#define MathLib_Image_PNG_Chunk_H
#include <stdint.h>

namespace MathLib {
    struct PNGChunk {
        uint32_t length;
        char type[4];

        PNGChunk(void);
        PNGChunk(uint32_t length, const char* signature);
        [[nodiscard]] bool IsCritical(void) const;
        [[nodiscard]] bool IsPublic(void) const;
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] bool IsSafeToCopy(void) const;
    } __attribute__((packed));
}

#endif