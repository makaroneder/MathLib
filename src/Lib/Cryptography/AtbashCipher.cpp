#include "AtbashCipher.hpp"

namespace MathLib {
    Expected<String> AtbashCipher::GetKey(const String& baseKey) const {
        StartAndReturnFromBenchmark(Expected<String>(String("-1 ") + baseKey));
    }
}