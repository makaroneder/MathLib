#include "IDATChunk.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    IDATChunk::IDATChunk(uint16_t length) : PNGChunk(length, expectedType) {
        EmptyBenchmark
    }
}