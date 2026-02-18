#ifndef MathLib_Cryptography_OneWayCipher_SHA1_H
#define MathLib_Cryptography_OneWayCipher_SHA1_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct SHA1 : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif