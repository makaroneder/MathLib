#ifndef MathLib_Font_H
#define MathLib_Font_H
#include <stddef.h>
#include <stdint.h>

namespace MathLib {
    struct Font {
        [[nodiscard]] virtual size_t GetWidth(void) const = 0;
        [[nodiscard]] virtual size_t GetHeight(void) const = 0;
        [[nodiscard]] virtual const uint8_t* GetGlyph(size_t glyph) const = 0;
    };
}

#endif