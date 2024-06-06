#include "Register.hpp"

Register::Register(uint64_t val) : value(val) {}
uint32_t Register::Get32(bool upper) const {
    return value >> (32 * upper);
}
uint16_t Register::Get16(bool upper) const {
    return Get32(false) >> (16 * upper);
}
uint8_t Register::Get8(bool upper) const {
    return Get16(false) >> (8 * upper);
}
void Register::Set32(uint32_t val, bool upper) {
    const uint8_t shift = 32 * upper;
    value = (value & ~((uint64_t)UINT32_MAX << shift)) | (uint64_t)val << shift;
}
void Register::Set16(uint16_t val, bool upper) {
    const uint8_t shift = 16 * upper;
    value = (value & ~((uint64_t)UINT16_MAX << shift)) | ((uint64_t)val << shift);
}
void Register::Set8(uint8_t val, bool upper) {
    const uint8_t shift = 8 * upper;
    value = (value & ~((uint64_t)UINT8_MAX << shift)) | ((uint64_t)val << shift);
}
String Register::ToString(String padding) const {
    return padding + ::ToString(value);
}