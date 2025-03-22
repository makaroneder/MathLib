#include "AffineCipher.hpp"
#include "../../Host.hpp"

namespace MathLib {
    Array<uint8_t> AffineCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        if (key.GetSize() != 2) ReturnFromBenchmark(Array<uint8_t>());
        const ssize_t a = key.At(0);
        const ssize_t b = key.At(1);
        Array<uint8_t> ret = Array<uint8_t>(data.GetSize());
        for (size_t i = 0; i < ret.GetSize(); i++) {
            const uint8_t x = data.At(i);
            ret.At(i) = encrypt ? (x * a + b) : ((x - b) / a);
        }
        ReturnFromBenchmark(ret);
    }
}