#include "FATExtendedBootRecord.hpp"

namespace MathLib {
    bool FATExtendedBootRecord::IsValid(void) const {
        return type == Type::NoID || type == Type::Normal;
    }
}