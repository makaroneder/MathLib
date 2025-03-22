#include "Card.hpp"

Card::Card(MathLib::Image* image) : image(image), free(true) {}
MathLib::String Card::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + MathLib::BoolToString(free);
}