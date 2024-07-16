#include "Color.hpp"

Color::Color(const uint32_t& h) : hex(h) {}
Color::Color(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) {
    rgba.r = r;
    rgba.g = g;
    rgba.b = b;
    rgba.a = a;
}
uint32_t BlendColor(const uint32_t& c1, const uint32_t& c2) {
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