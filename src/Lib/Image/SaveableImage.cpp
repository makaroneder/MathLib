#include "SaveableImage.hpp"

namespace MathLib {
    SaveableImage::SaveableImage(void) : Image() {}
    SaveableImage::SaveableImage(size_t width, size_t height) : Image(width, height) {}
}