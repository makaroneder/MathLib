#ifndef MathLib_Cryptography_OneWayCipher_Adler32_H
#define MathLib_Cryptography_OneWayCipher_Adler32_H
#include "../OneWayCipher.hpp"

namespace MathLib {
    struct Adler32 : OneWayCipher {
        static constexpr uint16_t modulo = 65521;
        [[nodiscard]] uint32_t Get(const Sequence<uint8_t>& data) const;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif