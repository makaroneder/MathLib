#include "Cipher.hpp"

namespace MathLib {
    Array<Array<uint8_t>> Cipher::CreateSignature(const Sequence<uint8_t>& hash, const CipherKey& key) const {
        return MakeArray<Array<uint8_t>>(Encrypt(hash, key));
    }
    bool Cipher::VerifySignature(const Sequence<Array<uint8_t>>& signature, const Sequence<uint8_t>& hash, const CipherKey& key) const {
        return signature.GetSize() == 1 && Decrypt(signature.AtUnsafe(0), key) == hash;
    }
    Array<uint8_t> Cipher::Decrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        return DecryptPartial(data, key, Interval<size_t>(0, SIZE_MAX));
    }
}