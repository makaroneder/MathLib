#include "Register.hpp"
#include "../Host.hpp"
#include "../String.hpp"

Register::Register(const uint64_t& val) : value(val) {}
uint32_t Register::Get32(const bool& upper) const {
    return value >> (32 * upper);
}
uint16_t Register::Get16(const bool& upper) const {
    return Get32(false) >> (16 * upper);
}
uint8_t Register::Get8(const bool& upper) const {
    return Get16(false) >> (8 * upper);
}
void Register::Set32(const uint32_t& val, const bool& upper) {
    const uint8_t shift = 32 * upper;
    value = (value & ~((uint64_t)UINT32_MAX << shift)) | (uint64_t)val << shift;
}
void Register::Set16(const uint16_t& val, const bool& upper) {
    const uint8_t shift = 16 * upper;
    value = (value & ~((uint64_t)UINT16_MAX << shift)) | ((uint64_t)val << shift);
}
void Register::Set8(const uint8_t& val, const bool& upper) {
    const uint8_t shift = 8 * upper;
    value = (value & ~((uint64_t)UINT8_MAX << shift)) | ((uint64_t)val << shift);
}
String Register::ToString(const String& padding) const {
    return padding + "0x" + ::ToString(value, 16);
}