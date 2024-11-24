#include "AtbashCipher.hpp"

namespace MathLib {
    Expected<String> AtbashCipher::GetKey(const String& baseKey) const {
        return Expected<String>(String("-1 ") + baseKey);
    }
}