#include "NestedCipherData.hpp"

namespace MathLib {
    NestedCipherData::NestedCipherData(Cipher* cipher, size_t keySize) : cipher(cipher), keySize(keySize) {}
    NestedCipherData::~NestedCipherData(void) {
        delete cipher;
    }
    size_t NestedCipherData::GetKeySize(void) const {
        return keySize;
    }
    Array<uint8_t> NestedCipherData::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        return key.GetSize() == keySize ? cipher->Encrypt(data, key) : Array<uint8_t>();
    }
    Array<uint8_t> NestedCipherData::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        return key.GetSize() == keySize ? cipher->DecryptPartial(data, key, range) : Array<uint8_t>();
    }
}