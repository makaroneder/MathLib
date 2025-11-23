#include "TGAHeader.hpp"

namespace MathLib {
    TGAHeader::TGAHeader(void) : idLength(0), colorMapType(0), imageType(ImageType::None), colorMapFirstEntryIndex(0), colorMapLength(0), colorMapEntrySize(0), xOrigin(0), yOrigin(0), width(0), height(0), pixelDepth(0), alphaDepth(0), rightLeft(false), topBottom(true), reserved(0) {}
    TGAHeader::TGAHeader(uint8_t idLength, ImageType imageType, uint16_t width, uint16_t height, uint8_t pixelDepth) : idLength(idLength), colorMapType(0), imageType(imageType), colorMapFirstEntryIndex(0), colorMapLength(0), colorMapEntrySize(0), xOrigin(0), yOrigin(height), width(width), height(height), pixelDepth(pixelDepth), alphaDepth(0), rightLeft(false), topBottom(true), reserved(0) {}
}