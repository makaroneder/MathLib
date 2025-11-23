#include "AsepriteCelChunk.hpp"

namespace MathLib {
    AsepriteCelChunk::AsepriteCelChunk(uint32_t size, Type type) : AsepriteChunkHeader(size + sizeof(AsepriteCelChunk) - sizeof(AsepriteChunkHeader), AsepriteChunkHeader::Type::Cel), layer(0), x(0), y(0), opacity(UINT8_MAX), type(type), z(0) {}
}