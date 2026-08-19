#ifndef MathLib_Image_PNG_H
#define MathLib_Image_PNG_H
#include "../SaveableImage.hpp"

namespace MathLib {
    struct PNG : SaveableImage {
        PNG(void);
        PNG(size_t width, size_t height);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif