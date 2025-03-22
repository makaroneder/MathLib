#include "AtbashCipher.hpp"

namespace MathLib {
    Expected<Array<uint64_t>> AtbashCipher::GetKey(const Sequence<uint64_t>& baseKey) const {
        StartAndReturnFromBenchmark(baseKey.GetSize() == 1 ? MakeArray<uint64_t>(-1, baseKey.At(0)) : Expected<Array<uint64_t>>());
    }
}