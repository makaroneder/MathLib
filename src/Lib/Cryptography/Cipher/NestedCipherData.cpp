#include "NestedCipherData.hpp"

namespace MathLib {
    NestedCipherData::NestedCipherData(Cipher* cipher, size_t keySize) : cipher(cipher), keySize(keySize) {
        EmptyBenchmark
    }
    NestedCipherData::~NestedCipherData(void) {
        StartBenchmark
        delete cipher;
        EndBenchmark
    }
    size_t NestedCipherData::GetKeySize(void) const {
        StartAndReturnFromBenchmark(keySize);
    }
    Array<uint8_t> NestedCipherData::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartAndReturnFromBenchmark(key.GetSize() == keySize ? cipher->Encrypt(data, key) : Array<uint8_t>());
    }
    Array<uint8_t> NestedCipherData::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartAndReturnFromBenchmark(key.GetSize() == keySize ? cipher->DecryptPartial(data, key, range) : Array<uint8_t>());

    }
}