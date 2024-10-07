#ifndef Color_H
#define Color_H
#include "Math/Constants.hpp"
#include "Interval.hpp"
#include <stdint.h>

/// @brief Union for converting 32 bit color to RGBA
union Color {
    /// @brief 32 bit color
    uint32_t hex;
    /// @brief Representation of 32 bit color in RGBA format
    struct RGBA {
        /// @brief Alpha channel
        uint8_t a;
        /// @brief Blue channel
        uint8_t b;
        /// @brief Green channel
        uint8_t g;
        /// @brief Red channel
        uint8_t r;
    } __attribute__((packed)) rgba;

    /// @brief Creates a new color union from a 32 bit color
    /// @param h 32 bit color
    Color(uint32_t h = 0);
    /// @brief Creates a new color union from a RGBA color
    /// @param r Red channel
    /// @param g Green channel
    /// @param b Blue channel
    /// @param a Alpha channel
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};
/// @brief R(t) = [255 * sin^2(t), 255 * sin^2(t + 0.66 * pi), 255 * sin^2(t + 1.32 * pi), 255]
/// @tparam T Type of number
/// @param t Rainbow index
/// @return Rainbow color
template <typename T>
uint32_t GetRainbow(const T& t) {
    return Color(UINT8_MAX * Pow(Sin(t), 2), UINT8_MAX * Pow(Sin(t + 0.66 * pi), 2), UINT8_MAX * Pow(Sin(t + 1.32 * pi), 2), UINT8_MAX).hex;
}
/// @brief B(a, b) = [a_0 * (255 - b_3) + b_0 * b_3) / 255, a_1 * (255 - b_3) + b_1 * b_3) / 255, a_2 * (255 - b_3) + b_2 * b_3) / 255, 255]
/// @param c1 Previus color
/// @param c2 New color
/// @return Blended colors
uint32_t BlendColor(uint32_t c1, uint32_t c2);

#endif