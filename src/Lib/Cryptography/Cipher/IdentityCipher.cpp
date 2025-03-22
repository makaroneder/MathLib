#include "IdentityCipher.hpp"

namespace MathLib {
    Array<uint8_t> IdentityCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool) const {
        StartAndReturnFromBenchmark(key.IsEmpty() ? CollectionToArray(data) : Array<uint8_t>());
    }
}