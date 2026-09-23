#include "Int24.hpp"
#include <Host.hpp>

Int24::Int24(void) : data { 0, 0, 0, } {}
Int24::Int24(uint32_t value) : data { (uint8_t)(value >> 16), (uint8_t)(value >> 8), (uint8_t)value } {
    if (value >> 24) MathLib::Panic("Overflow");
}
uint32_t Int24::Get(void) const {
    return (uint32_t)data[0] << 16 | (uint16_t)data[1] << 8 | data[2];
}
Int24 Int24::SwapEndian(void) const {
    Int24 ret = *this;
    MathLib::Swap<uint8_t>(ret.data[0], ret.data[2]);
    return ret;
}