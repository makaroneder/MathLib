#ifndef MathLib_Image_PNG_IDATChunk_H
#define MathLib_Image_PNG_IDATChunk_H
#include "PNGChunk.hpp"

namespace MathLib {
    struct IDATChunk : PNGChunk {
        static constexpr const char* expectedType = "IDAT";
        uint8_t data[];

        IDATChunk(uint16_t length);
    } __attribute__((packed));
}

#endif