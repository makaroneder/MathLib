#include "AtbashCipher.hpp"

namespace MathLib {
    Expected<String> AtbashCipher::GetKey(const String& baseKey) const {
        StartBenchmark
        ReturnFromBenchmark(Expected<String>(String("-1 ") + baseKey));
    }
}