#include "ROT13.hpp"

namespace MathLib {
    Expected<Array<uint64_t>> ROT13::GetKey(const Sequence<uint64_t>& baseKey) const {
        StartAndReturnFromBenchmark(baseKey.IsEmpty() ? MathLib::MakeArray<uint64_t>(13) : Expected<Array<uint64_t>>());
    }
}