#ifndef MathLib_Image_H
#define MathLib_Image_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    struct Image : Allocatable {
        /// @brief Creates a new image
        /// @param width Width of the image
        /// @param height Height of the image
        Image(size_t width, size_t height);
        /// @brief Returns width of the image
        /// @return Width of the image
        [[nodiscard]] size_t GetWidth(void) const;
        /// @brief Returns height of the image
        /// @return Height of the image
        [[nodiscard]] size_t GetHeight(void) const;
        [[nodiscard]] size_t GetSize(void) const;
        /// @brief Fills image
        /// @param color Color to fill the image with
        void Fill(uint32_t color);
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] uint32_t& At(size_t x, size_t y);
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] uint32_t At(size_t x, size_t y) const;
        [[nodiscard]] Image Resize(size_t xMultiplier, size_t yMultiplier) const;
        [[nodiscard]] bool operator==(const Image& other) const;
        [[nodiscard]] bool operator!=(const Image& other) const;

        /// @brief Pixels
        Matrix<uint32_t> pixels;
    };
}

#endif