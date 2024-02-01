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