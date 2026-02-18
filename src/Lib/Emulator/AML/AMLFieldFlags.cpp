#include "AMLFieldFlags.hpp"

namespace MathLib {
    AMLFieldFlags::AMLFieldFlags(uint8_t value) : value(value) {}
    bool AMLFieldFlags::operator==(const AMLFieldFlags& other) const {
        return value == other.value;
    }
    bool AMLFieldFlags::operator!=(const AMLFieldFlags& other) const {
        return !(*this == other);
    }
}