#include "QuakePakFileEntry.hpp"

namespace MathLib {
    String QuakePakFileEntry::GetName(void) const {
        String ret;
        for (uint8_t i = 0; i < SizeOfArray(name); i++) {
            if (!name[i]) break;
            ret += name[i];
        }
        return ret;
    }
}