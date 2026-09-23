#include "TLSRandom.hpp"

bool TLSRandom::operator==(const TLSRandom& other) const {
    for (uint8_t i = 0; i < sizeof(random); i++)
        if (random[i] != other.random[i]) return false;
    return true;
}
bool TLSRandom::operator!=(const TLSRandom& other) const {
    return !(*this == other);
}