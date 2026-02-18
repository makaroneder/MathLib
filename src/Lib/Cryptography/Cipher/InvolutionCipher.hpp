#ifndef MathLib_Cryptography_Cipher_Involution_H
#define MathLib_Cryptography_Cipher_Involution_H
#include "../Cipher.hpp"

namespace MathLib {
    struct InvolutionCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Convert(const Sequence<uint8_t>& data, const CipherKey& key, size_t limit) const = 0;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;
    };
}

#endif