#include "FATLongFileName.hpp"

namespace MathLib {
    bool FATLongFileName::IsValid(void) const {
        return signature == expectedSignature;
    }
}