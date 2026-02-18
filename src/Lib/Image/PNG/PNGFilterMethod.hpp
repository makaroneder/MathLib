#ifndef MathLib_Image_PNG_FilterMethod_H
#define MathLib_Image_PNG_FilterMethod_H
#include <stdint.h>

namespace MathLib {
    enum class PNGFilterMethod : uint8_t {
        None = 0,
        Sub,
        Up,
        Average,
        Paeth,
    };
}

#endif