#ifndef MathLib_Cryptography_OneWayCipher_InternetChecksum_H
#define MathLib_Cryptography_OneWayCipher_InternetChecksum_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct InternetChecksum : OneWayCipher {
        [[nodiscard]] uint16_t Get(const Sequence<uint8_t>& data) const;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif