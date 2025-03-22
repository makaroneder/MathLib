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
    Array<uint8_t> NestedCipherData::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartAndReturnFromBenchmark(key.GetSize() == keySize ? cipher->Encrypt(data, key, encrypt) : Array<uint8_t>());
    }
}