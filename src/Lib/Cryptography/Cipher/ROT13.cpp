#include "ROT13.hpp"

namespace MathLib {
    Expected<Array<uint64_t>> ROT13::GetKey(const Sequence<uint64_t>& baseKey) const {
        return baseKey.IsEmpty() ? MakeArray<uint64_t>(13) : Expected<Array<uint64_t>>();
    }
}