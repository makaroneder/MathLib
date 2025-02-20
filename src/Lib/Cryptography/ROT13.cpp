#include "ROT13.hpp"

namespace MathLib {
    Expected<String> ROT13::GetKey(const String& baseKey) const {
        StartAndReturnFromBenchmark(baseKey.IsEmpty() ? Expected<String>("13") : Expected<String>());
    }
}