#ifndef MathLib_Image_TGA_H
#define MathLib_Image_TGA_H
#include "../SaveableImage.hpp"

namespace MathLib {
    struct TGA : SaveableImage {
        TGA(size_t width, size_t height);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif