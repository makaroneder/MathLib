#ifndef MathLib_Image_Frame_H
#define MathLib_Image_Frame_H
#include "Image.hpp"

namespace MathLib {
    struct Frame : Image {
        num_t duration;

        Frame(void);
        Frame(size_t width, size_t height, const num_t& duration);
        Frame(const Image& image, const num_t& duration);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] bool operator==(const Frame& other) const;
        [[nodiscard]] bool operator!=(const Frame& other) const;
    };
}

#endif