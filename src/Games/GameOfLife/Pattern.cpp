#include "Pattern.hpp"

Pattern::Pattern(const MathLib::Matrix<uint8_t>& data, const MathLib::Matrix<ssize_t>& position) : data(data), position(position) {}
MathLib::Matrix<ssize_t> Pattern::GetPosition(void) const {
    return position;
}
size_t Pattern::GetWidth(void) const {
    return data.GetWidth();
}
size_t Pattern::GetHeight(void) const {
    return data.GetHeight();
}
uint8_t Pattern::At(size_t x, size_t y) const {
    return data.At(x, y);
}
uint8_t& Pattern::At(size_t x, size_t y) {
    return data.At(x, y);
}