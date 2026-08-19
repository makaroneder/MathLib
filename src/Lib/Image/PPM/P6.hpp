#ifndef MathLib_Image_PPM_P6_H
#define MathLib_Image_PPM_P6_H
#include "../SaveableImage.hpp"

namespace MathLib {
    // TODO: Add support for P7, P3, P2, P5, P1, P4 and PF
    struct P6 : SaveableImage {
        P6(void);
        P6(size_t width, size_t height);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif