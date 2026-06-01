#include "ELFRelocationWithAddend.hpp"

namespace MathLib {
    bool ELFRelocationWithAddend::operator==(const ELFRelocationWithAddend& other) const {
        return ELFRelocation::operator==(other) && addend == other.addend;
    }
    bool ELFRelocationWithAddend::operator!=(const ELFRelocationWithAddend& other) const {
        return !(*this == other);
    }
}