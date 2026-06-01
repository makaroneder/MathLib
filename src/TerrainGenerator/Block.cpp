#include "Block.hpp"

Block::Block(void) : options(), checked(false) {}
Block::Block(const MathLib::Array<uint8_t>& options) : options(options), checked(false) {}
Block::Block(uint8_t maxOption) : options(maxOption), checked(false) {
    for (uint8_t i = 0; i < maxOption; i++) options.AtUnsafe(i) = i;
}
size_t Block::GetOptionCount(void) const {
    return checked ? 0 : options.GetSize();
}
uint8_t Block::Get(void) const {
    return checked ? options.At(0) : UINT8_MAX;
}
uint8_t Block::GetRandomOption(void) {
    if (checked) return UINT8_MAX;
    const uint8_t value = MathLib::AtRandom<uint8_t>(options);
    options = MathLib::MakeArray<uint8_t>(value);
    checked = true;
    return value;
}
bool Block::Intersect(const Block& other) {
    if (checked) return true;
    MathLib::Array<uint8_t> newOptions;
    for (uint8_t option : options)
        if (other.options.Contains(option) && !newOptions.Add(option)) return false;
    options = newOptions;
    return true;
}
MathLib::String Block::ToString(const MathLib::Sequence<char>& padding) const {
    const size_t size = options.GetSize();
    MathLib::String ret = MathLib::CollectionToString(padding) + '[';
    for (size_t i = 0; i < size; i++) {
        if (i) ret += ", ";
        ret += MathLib::ToString(options.AtUnsafe(i), 10);
    }
    return ret + ']';
}