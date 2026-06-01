#ifndef MathLib_Font_PSF2_H
#define MathLib_Font_PSF2_H
#include "../Interfaces/Saveable.hpp"
#include "Font.hpp"

namespace MathLib {
    struct PSF2 : Font, Saveable {
        [[nodiscard]] virtual size_t GetWidth(void) const override;
        [[nodiscard]] virtual size_t GetHeight(void) const override;
        [[nodiscard]] virtual const uint8_t* GetGlyph(size_t glyph) const override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;

        private:
        Array<uint8_t> glyphs;
        size_t width;
        size_t height;
        size_t bytesPerGlyph;
    };
}

#endif