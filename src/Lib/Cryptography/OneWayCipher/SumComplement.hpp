#ifndef MathLib_Cryptography_OneWayCipher_SumComplement_H
#define MathLib_Cryptography_OneWayCipher_SumComplement_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct SumComplement : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif