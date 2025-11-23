#include "PNGChunk.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    PNGChunk::PNGChunk(void) {}
    PNGChunk::PNGChunk(uint32_t length, const char* signature) : length(SwapEndian32(length)) {
        for (uint8_t i = 0; i < SizeOfArray(type); i++) type[i] = signature[i];
    }
    bool PNGChunk::IsCritical(void) const {
        return IsUpper(type[0]);
    }
    bool PNGChunk::IsPublic(void) const {
        return IsUpper(type[1]);
    }
    bool PNGChunk::IsValid(void) const {
        return IsUpper(type[2]);
    }
    bool PNGChunk::IsSafeToCopy(void) const {
        return IsLower(type[3]);
    }
}