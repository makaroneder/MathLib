#include "Color.hpp"

Color::Color(uint32_t h) {
    hex = h;
}
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    rgba.r = r;
    rgba.g = g;
    rgba.b = b;
    rgba.a = a;
}
uint32_t BlendColor(uint32_t c1, uint32_t c2) {
    Color color1 = Color(c1);
    Color color2 = Color(c2);
    return Color(
        ((size_t)color1.rgba.r * (UINT8_MAX - (size_t)color2.rgba.a) + (size_t)color2.rgba.r * (size_t)color2.rgba.a) / UINT8_MAX,
        ((size_t)color1.rgba.g * (UINT8_MAX - (size_t)color2.rgba.a) + (size_t)color2.rgba.g * (size_t)color2.rgba.a) / UINT8_MAX,
        ((size_t)color1.rgba.b * (UINT8_MAX - (size_t)color2.rgba.a) + (size_t)color2.rgba.b * (size_t)color2.rgba.a) / UINT8_MAX,
        color1.rgba.a
    ).hex;
}