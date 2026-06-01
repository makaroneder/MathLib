#include "AsepriteLayerChunk.hpp"

namespace MathLib {
    AsepriteLayerChunk::AsepriteLayerChunk(uint16_t width, uint16_t height) : AsepriteChunkHeader(sizeof(AsepriteLayerChunk) - sizeof(AsepriteChunkHeader), AsepriteChunkHeader::Type::Layer), flags((1 << (uint8_t)Flag::Visible) | (1 << (uint8_t)Flag::Editable)), type(Type::Normal), childLevel(0), defaultWidth(width), defaultHeight(height), blendMode(BlendMode::Normal), opacity(UINT8_MAX), name() {}
}