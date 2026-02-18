#ifndef MathLib_Cryptography_OneWayCipher_BSDChecksum_H
#define MathLib_Cryptography_OneWayCipher_BSDChecksum_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct BSDChecksum : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif