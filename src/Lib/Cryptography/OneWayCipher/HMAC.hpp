#ifndef MathLib_Cryptography_OneWayCipher_HMAC_H
#define MathLib_Cryptography_OneWayCipher_HMAC_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct HMAC : OneWayCipher {
        static constexpr uint8_t outerPaddingValue = 0x5c;
        static constexpr uint8_t innerPaddingValue = 0x36;
        enum class BlockSize : size_t {
            MD5 = 64,
            SHA1 = MD5,
            SHA224 = MD5,
            SHA256 = MD5,
            SHA3512 = 72,
            SHA3384 = 104,
            SHA512224 = 128,
            SHA512256 = SHA512224,
            SHA384 = SHA512224,
            SHA512 = SHA512224,
            SHA3256 = 136,
            SHA3224 = 144,
        };

        HMAC(OneWayCipher& hash);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;

        private:
        OneWayCipher& hash;
    };
}

#endif