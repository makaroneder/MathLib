#include "CaesarCipher.hpp"

namespace MathLib {
    Expected<String> CaesarCipher::GetKey(const String& baseKey) const {
        StartBenchmark
        ReturnFromBenchmark(Expected<String>(String("1 ") + baseKey));
    }
}