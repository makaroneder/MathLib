#include "IDATChunk.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    IDATChunk::IDATChunk(uint16_t length) : PNGChunk(length, expectedType) {
        EmptyBenchmark
    }
}