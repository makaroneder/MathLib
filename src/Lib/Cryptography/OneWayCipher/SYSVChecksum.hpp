#ifndef MathLib_Cryptography_OneWayCipher_SYSVChecksum_H
#define MathLib_Cryptography_OneWayCipher_SYSVChecksum_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct SYSVChecksum : OneWayCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
    };
}

#endif