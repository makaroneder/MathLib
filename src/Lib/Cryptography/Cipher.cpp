#include "Cipher.hpp"

namespace MathLib {
    Array<uint8_t> Cipher::Decrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        return DecryptPartial(data, key, Interval<size_t>(0, SIZE_MAX));
    }
}