#include "Card.hpp"

Card::Card(MathLib::Image* image) : image(image), free(true) {}
MathLib::String Card::ToString(const MathLib::String& padding) const {
    return padding + MathLib::BoolToString(free);
}