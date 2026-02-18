#ifndef MathLib_Cryptography_OneWayCipher_BIP8_H
#define MathLib_Cryptography_OneWayCipher_BIP8_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct BIP8 : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif