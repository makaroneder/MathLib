#include "Color.hpp"

namespace MathLib {
    Color::RGBA::RGBA(void) : a(0), b(0), g(0), r(0) {
        EmptyBenchmark
    }
    Color::RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : a(a), b(b), g(g), r(r) {
        EmptyBenchmark
    }
    Color::Color(uint32_t h) : hex(h) {
        EmptyBenchmark
    }
    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : rgba(r, g, b, a) {
        EmptyBenchmark
    }
    uint32_t BlendColor(uint32_t c1, uint32_t c2) {
        StartBenchmark
        const Color color1 = Color(c1);
        const Color color2 = Color(c2);
        const num_t alpha = color2.rgba.a / UINT8_MAX;
        const num_t invAlpha = 1 - alpha;
        ReturnFromBenchmark(Color(
            (uint8_t)(color2.rgba.r * alpha + color1.rgba.r * invAlpha),
            (uint8_t)(color2.rgba.g * alpha + color1.rgba.g * invAlpha),
            (uint8_t)(color2.rgba.b * alpha + color1.rgba.b * invAlpha),
            (uint8_t)(color2.rgba.a + color1.rgba.a * invAlpha)
        ).hex);
    }
}