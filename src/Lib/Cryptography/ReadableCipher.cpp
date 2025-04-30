#include "ReadableCipher.hpp"
#include "../Interfaces/ByteArray.hpp"

namespace MathLib {
    Array<uint8_t> ReadableCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        ByteArray byteArray = ByteArray(data);
        ReturnFromBenchmark(DecryptReadablePartial(byteArray, key, range));
    }
    Array<uint8_t> ReadableCipher::DecryptReadable(Readable& readable, const Sequence<uint64_t>& key) const {
        StartAndReturnFromBenchmark(DecryptReadablePartial(readable, key, Interval<size_t>(0, SIZE_MAX)));
    }
}