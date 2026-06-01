#ifndef MathLib_Font_PSF1_H
#define MathLib_Font_PSF1_H
#include "../Interfaces/Saveable.hpp"
#include "Font.hpp"

namespace MathLib {
    struct PSF1 : Font, Saveable {
        static constexpr size_t width = 8;

        [[nodiscard]] virtual size_t GetWidth(void) const override;
        [[nodiscard]] virtual size_t GetHeight(void) const override;
        [[nodiscard]] virtual const uint8_t* GetGlyph(size_t glyph) const override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;

        private:
        Array<uint8_t> glyphs;
        size_t height;
    };
}

#endif