#include "ELFRelocation.hpp"

namespace MathLib {
    bool ELFRelocation::operator==(const ELFRelocation& other) const {
        return address == other.address && type == other.type && symbol == other.symbol;
    }
    bool ELFRelocation::operator!=(const ELFRelocation& other) const {
        return !(*this == other);
    }
}