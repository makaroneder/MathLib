#include "XZFlags.hpp"

namespace MathLib {
    XZFlags::XZFlags(void) : flags1(0), checkType((uint8_t)CheckType::None), flags2(0) {}
    XZFlags::XZFlags(CheckType type) : flags1(0), checkType((uint8_t)type), flags2(0) {}
    uint8_t XZFlags::GetSizeOfCheck(CheckType type) {
        switch ((uint8_t)type) {
            case 0: return 0;
            case 1 ... 3: return 4;
            case 4 ... 6: return 8;
            case 7 ... 9: return 16;
            case 10 ... 12: return 32;
            case 13 ... 15: return 64;
            default: return UINT8_MAX;
        }
    }
    ExternArray<uint8_t> XZFlags::GetFlags(void) const {
        return ExternArray<uint8_t>((uint8_t*)&flags1, sizeof(uint16_t));
    }
    bool XZFlags::operator==(const XZFlags& other) const {
        return flags1 == other.flags1 && checkType == other.checkType && flags2 == other.flags2;
    }
    bool XZFlags::operator!=(const XZFlags& other) const {
        return !(*this == other);
    }
}