#ifndef MathLib_Image_PNG_IENDChunk_H
#define MathLib_Image_PNG_IENDChunk_H
#include "PNGChunk.hpp"

namespace MathLib {
    struct IENDChunk : PNGChunk {
        static constexpr const char* expectedType = "IEND";

        IENDChunk(void);
    } __attribute__((packed));
}

#endif