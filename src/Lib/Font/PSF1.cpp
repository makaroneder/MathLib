#include "PSF1.hpp"
#include "PSF1Header.hpp"

namespace MathLib {
    size_t PSF1::GetWidth(void) const {
        return width;
    }
    size_t PSF1::GetHeight(void) const {
        return height;
    }
    const uint8_t* PSF1::GetGlyph(size_t glyph) const {
        const size_t i = glyph * height;
        return i < glyphs.GetSize() ? glyphs.GetValue() + i : nullptr;
    }
    bool PSF1::Save(Writable& file) const {
        const PSF1Header header = PSF1Header(height, glyphs.GetSize() == 512);
        return file.Write<PSF1Header>(header) && file.WriteBuffer(glyphs.GetValue(), glyphs.GetSize());
    }
    bool PSF1::Load(Readable& file) {
        PSF1Header header;
        if (!file.Read<PSF1Header>(header) || !header.IsValid()) return false;
        height = header.charSize;
        const uint32_t size = header.GetGlyphCount() * header.charSize;
        glyphs = size;
        return file.ReadBuffer(glyphs.GetValue(), size);
    }
}