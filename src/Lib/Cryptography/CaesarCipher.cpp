#include "CaesarCipher.hpp"

namespace MathLib {
    Expected<String> CaesarCipher::GetKey(const String& baseKey) const {
        StartAndReturnFromBenchmark(Expected<String>(String("1 ") + baseKey));
    }
}