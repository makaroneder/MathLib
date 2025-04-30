#include "PNGChunk.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    PNGChunk::PNGChunk(void) {
        EmptyBenchmark
    }
    PNGChunk::PNGChunk(uint32_t length, const char* signature) : length(SwapEndian32(length)) {
        StartBenchmark
        for (uint8_t i = 0; i < SizeOfArray(type); i++) type[i] = signature[i];
        EndBenchmark
    }
    bool PNGChunk::IsCritical(void) const {
        StartAndReturnFromBenchmark(IsUpper(type[0]));
    }
    bool PNGChunk::IsPublic(void) const {
        StartAndReturnFromBenchmark(IsUpper(type[1]));
    }
    bool PNGChunk::IsValid(void) const {
        StartAndReturnFromBenchmark(IsUpper(type[2]));
    }
    bool PNGChunk::IsSafeToCopy(void) const {
        StartAndReturnFromBenchmark(IsLower(type[3]));
    }
}