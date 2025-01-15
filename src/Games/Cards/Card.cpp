#include "Card.hpp"

Card::Card(MathLib::Image* image) : image(image), free(true) {}
MathLib::String Card::ToString(const MathLib::String& padding) const {
    return padding + "Image: 0x" + MathLib::ToString((uintptr_t)image, 16);
}