#include "Card.hpp"

Card::Card(MathLib::Image* image) : image(image), free(true) {}
MathLib::String Card::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + MathLib::BoolToString(free);
}
bool Card::operator==(const Card& other) const {
    return free == other.free && *image == *other.image;
}
bool Card::operator!=(const Card& other) const {
    return !(*this == other);
}