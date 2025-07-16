#include "IENDChunk.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    IENDChunk::IENDChunk(void) : PNGChunk(0, expectedType) {
        EmptyBenchmark
    }
}