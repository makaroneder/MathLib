#ifndef Color_H
#define Color_H
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
    } rgba;

    /// @brief Creates a new color union from a 32 bit color
    /// @param h 32 bit color
    Color(uint32_t h);
    /// @brief Creates a new color union from a RGBA color
    /// @param r Red channel
    /// @param g Green channel
    /// @param b Blue channel
    /// @param a Alpha channel
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

#endif