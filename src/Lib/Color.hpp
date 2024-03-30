#ifndef Color_H
#define Color_H
#include <stdint.h>
#include "Constants.hpp"

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
    } rgba;

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
template <typename T>
static uint32_t GetRainbow(T t) {
    return Color(UINT8_MAX * std::pow(std::sin(t), 2), UINT8_MAX * std::pow(std::sin(t + 0.33 * 2 * pi), 2), UINT8_MAX * std::pow(std::sin(t + 0.66 * 2 * pi), 2), UINT8_MAX).hex;
}

#endif