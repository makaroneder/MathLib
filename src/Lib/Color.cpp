#include "Color.hpp"

namespace MathLib {
    Color::RGBA::RGBA(void) : a(0), b(0), g(0), r(0) {}
    Color::RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : a(a), b(b), g(g), r(r) {}
    Color::Color(uint32_t h) : hex(h) {}
    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : rgba(r, g, b, a) {}
    uint32_t BlendColor(uint32_t c1, uint32_t c2) {
        const Color color1 = Color(c1);
        const Color color2 = Color(c2);
        const num_t alpha = color2.rgba.a / UINT8_MAX;
        const num_t invAlpha = 1 - alpha;
        return Color(
            (uint8_t)(color2.rgba.r * alpha + color1.rgba.r * invAlpha),
            (uint8_t)(color2.rgba.g * alpha + color1.rgba.g * invAlpha),
            (uint8_t)(color2.rgba.b * alpha + color1.rgba.b * invAlpha),
            (uint8_t)(color2.rgba.a + color1.rgba.a * invAlpha)
        ).hex;
    }
    uint32_t BlendColor(uint32_t c1, uint32_t c2, uint8_t alphaPosition) {
        const uint8_t* color1 = (const uint8_t*)&c1;
        const uint8_t* color2 = (const uint8_t*)&c2;
        const num_t alpha = color2[alphaPosition] / UINT8_MAX;
        const num_t invAlpha = 1 - alpha;
        uint8_t ret[sizeof(uint32_t)];
        for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
            if (i == alphaPosition) continue;
            ret[i] = color2[i] * alpha + color1[i] * invAlpha;
        }
        ret[alphaPosition] = color2[alphaPosition] + color1[alphaPosition] * invAlpha;
        return *(const uint32_t*)ret;
    }
}