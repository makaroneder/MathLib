#include "AsepriteLayerChunk.hpp"

namespace MathLib {
    AsepriteLayerChunk::AsepriteLayerChunk(uint16_t width, uint16_t height) : AsepriteChunkHeader(sizeof(AsepriteLayerChunk) - sizeof(AsepriteChunkHeader), AsepriteChunkHeader::Type::Layer), visible(true), editable(true), lockMovement(false), background(false), preferLinkedCels(false), displayCollapsed(false), referenceLayer(false), reserved1(0), type(Type::Normal), childLevel(0), defaultWidth(width), defaultHeight(height), blendMode(BlendMode::Normal), opacity(UINT8_MAX), name() {}
}