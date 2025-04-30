#include "AsepriteChunkHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    AsepriteChunkHeader::AsepriteChunkHeader(uint32_t size, Type type) : size(size + sizeof(AsepriteChunkHeader)), type(type) {
        EmptyBenchmark
    }
}