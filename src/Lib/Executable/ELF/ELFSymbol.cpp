#include "ELFSymbol.hpp"

namespace MathLib {
    bool ELFSymbol::operator==(const ELFSymbol& other) const {
        return name == other.name && type == other.type && bind == other.bind && visibility == other.visibility && reserved == other.reserved && section == other.section && value == other.value && size == other.size;
    }
    bool ELFSymbol::operator!=(const ELFSymbol& other) const {
        return !(*this == other);
    }
}