#ifndef MathLib_Color_H
#define MathLib_Color_H
#include "Math/Constants.hpp"
#include "Math/Matrix.hpp"
#include "Interval.hpp"

namespace MathLib {
    union Color {
        uint32_t hex;
        struct RGBA {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;

            RGBA(void);
            RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        } __attribute__((packed)) rgba;

        Color(uint32_t h = 0);
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        template <typename T>
        [[nodiscard]] static Color FromVector(const Matrix<T>& v) {
            return Color(GetX(v) * UINT8_MAX, GetY(v) * UINT8_MAX, GetZ(v) * UINT8_MAX, v.At(3, 0) * UINT8_MAX);
        }
    };
    template <typename T>
    [[nodiscard]] uint32_t GetRainbow(const T& t) {
        const T t0 = Sin(t);
        const T t1 = Sin(t + 0.66 * pi);
        const T t2 = Sin(t + 1.32 * pi);
        return Color(UINT8_MAX * t0 * t0, UINT8_MAX * t1 * t1, UINT8_MAX * t2 * t2, UINT8_MAX).hex;
    }
    [[nodiscard]] uint32_t BlendColor(uint32_t c1, uint32_t c2);
    [[nodiscard]] uint32_t BlendColor(uint32_t c1, uint32_t c2, uint8_t alphaPosition);
}

#endif