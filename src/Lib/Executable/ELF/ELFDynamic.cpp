#include "ELFDynamic.hpp"

namespace MathLib {
    bool ELFDynamic::operator==(const ELFDynamic& other) const {
        return type == other.type && value == other.value;
    }
    bool ELFDynamic::operator!=(const ELFDynamic& other) const {
        return !(*this == other);
    }
}