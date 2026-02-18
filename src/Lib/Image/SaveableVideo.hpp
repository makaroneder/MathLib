#ifndef MathLib_Image_SaveableVideo_H
#define MathLib_Image_SaveableVideo_H
#include "Video.hpp"

namespace MathLib {
    struct SaveableVideo : Video, Saveable {};
}

#endif