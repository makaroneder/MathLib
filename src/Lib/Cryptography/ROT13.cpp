#include "ROT13.hpp"

namespace MathLib {
    Expected<String> ROT13::GetKey(const String& baseKey) const {
        return baseKey.IsEmpty() ? Expected<String>("13") : Expected<String>();
    }
}