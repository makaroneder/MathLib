#ifndef MathLib_Image_Saveable_H
#define MathLib_Image_Saveable_H
#include "Image.hpp"

namespace MathLib {
    struct SaveableImage : Image, Saveable {
        /// @brief Creates a new image
        /// @param width Width of the image
        /// @param height Height of the image
        SaveableImage(size_t width, size_t height);
    };
}

#endif