#ifndef MathLib_Color_H
#define MathLib_Color_H
#include "Math/Constants.hpp"
#include "Math/Matrix.hpp"
#include "Interval.hpp"

namespace MathLib {
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

            RGBA(void);
            RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
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
        template <typename T>
        [[nodiscard]] static Color FromVector(const Matrix<T>& v) {
            return Color(GetX(v) * UINT8_MAX, GetY(v) * UINT8_MAX, GetZ(v) * UINT8_MAX, v.At(3, 0) * UINT8_MAX);
        }
    };
    /// @brief R(t) = [255 * sin^2(t), 255 * sin^2(t + 0.66 * pi), 255 * sin^2(t + 1.32 * pi), 255]
    /// @tparam T Type of number
    /// @param t Rainbow index
    /// @return Rainbow color
    template <typename T>
    [[nodiscard]] uint32_t GetRainbow(const T& t) {
        const T t0 = Sin(t);
        const T t1 = Sin(t + 0.66 * pi);
        const T t2 = Sin(t + 1.32 * pi);
        return Color(UINT8_MAX * t0 * t0, UINT8_MAX * t1 * t1, UINT8_MAX * t2 * t2, UINT8_MAX).hex;
    }
    /// @brief B(a, b) = [a_0 * (255 - b_3) + b_0 * b_3) / 255, a_1 * (255 - b_3) + b_1 * b_3) / 255, a_2 * (255 - b_3) + b_2 * b_3) / 255, 255]
    /// @param c1 Previus color
    /// @param c2 New color
    /// @return Blended colors
    [[nodiscard]] uint32_t BlendColor(uint32_t c1, uint32_t c2);
    [[nodiscard]] uint32_t BlendColor(uint32_t c1, uint32_t c2, uint8_t alphaPosition);
}

#endif