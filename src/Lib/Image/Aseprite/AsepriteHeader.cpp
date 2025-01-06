#include "AsepriteHeader.hpp"

namespace MathLib {
    bool AsepriteHeader::IsValid(void) const {
        return signature == expectedSignature;
    }
}