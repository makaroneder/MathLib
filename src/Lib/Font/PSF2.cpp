#include "PSF2.hpp"
#include "PSF2Header.hpp"

namespace MathLib {
    size_t PSF2::GetWidth(void) const {
        return width;
    }
    size_t PSF2::GetHeight(void) const {
        return height;
    }
    const uint8_t* PSF2::GetGlyph(size_t glyph) const {
        const size_t i = glyph * bytesPerGlyph;
        return i < glyphs.GetSize() ? glyphs.GetValue() + i : nullptr;
    }
    bool PSF2::Save(Writable& file) const {
        const PSF2Header header = PSF2Header(width, height, glyphs.GetSize(), bytesPerGlyph);
        return file.Write<PSF2Header>(header) && file.WriteBuffer(glyphs.GetValue(), glyphs.GetSize());
    }
    bool PSF2::Load(Readable& file) {
        PSF2Header header;
        if (!file.Read<PSF2Header>(header) || !header.IsValid()) return false;
        width = header.width;
        height = header.height;
        bytesPerGlyph = header.bytesPerGlyph;
        const uint32_t size = header.glyphCount * header.bytesPerGlyph;
        glyphs = size;
        return file.ReadBuffer(glyphs.GetValue(), size);
    }
}