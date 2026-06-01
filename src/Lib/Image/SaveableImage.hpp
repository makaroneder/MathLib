#ifndef MathLib_Image_Saveable_H
#define MathLib_Image_Saveable_H
#include "Image.hpp"

namespace MathLib {
    struct SaveableImage : Image, Saveable {
        SaveableImage(size_t width, size_t height);
    };
}

#endif