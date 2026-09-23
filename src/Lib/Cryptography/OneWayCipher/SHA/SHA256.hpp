#ifndef MathLib_Cryptography_OneWayCipher_SHA256_H
#define MathLib_Cryptography_OneWayCipher_SHA256_H
#include "../../OneWayCipher.hpp"

namespace MathLib {
    struct SHA256 : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif