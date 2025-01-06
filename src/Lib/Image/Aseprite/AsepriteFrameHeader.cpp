#include "AsepriteFrameHeader.hpp"

namespace MathLib {
    bool AsepriteFrameHeader::IsValid(void) const {
        return signature == expectedSignature;
    }
}