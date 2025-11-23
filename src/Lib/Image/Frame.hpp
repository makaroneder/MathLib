#ifndef MathLib_Frame_H
#define MathLib_Frame_H
#include "Image.hpp"

namespace MathLib {
    struct Frame : Image {
        Frame(void);
        /// @brief Creates a new frame
        /// @param width Width of the frame
        /// @param height Height of the frame
        Frame(size_t width, size_t height, const num_t& duration);
        Frame(const Image& image, const num_t& duration);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] num_t GetDuration(void) const;
        [[nodiscard]] bool operator==(const Frame& other) const;
        [[nodiscard]] bool operator!=(const Frame& other) const;

        private:
        num_t duration;
    };
}

#endif