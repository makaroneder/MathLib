#ifndef Image_H
#define Image_H
#include "Matrix.hpp"

struct Image : Saveable {
    /// @brief Creates a new image
    /// @param w Width of the image
    /// @param h Height of the image
    Image(const size_t& w, const size_t& h);
    /// @brief Returns width of the image
    /// @return Width of the image
    size_t GetWidth(void) const;
    /// @brief Returns height of the image
    /// @return Height of the image
    size_t GetHeight(void) const;
    /// @brief Fills image
    /// @param color Color to fill the image with
    void Fill(const uint32_t& color);

    /// @brief Pixels
    Matrix<uint32_t> pixels;
};

#endif