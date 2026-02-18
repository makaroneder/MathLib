#include "ReadableCipher.hpp"
#include "../Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    Array<uint8_t> ReadableCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        ByteArray byteArray = ByteArray(data);
        return DecryptReadablePartial(byteArray, key, range);
    }
    Array<uint8_t> ReadableCipher::DecryptReadable(Readable& readable, const CipherKey& key) const {
        return DecryptReadablePartial(readable, key, Interval<size_t>(0, SIZE_MAX));
    }
}