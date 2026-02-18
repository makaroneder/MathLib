#include "FATExtendedBootRecord32.hpp"

namespace MathLib {
    bool FATExtendedBootRecord32::IsValid(void) const {
        return ebr.IsValid();
    }
}