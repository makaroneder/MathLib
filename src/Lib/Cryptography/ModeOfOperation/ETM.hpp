#ifndef MathLib_Cryptography_ModeOfOperation_ETM_H
#define MathLib_Cryptography_ModeOfOperation_ETM_H
#include "../Cipher.hpp"

namespace MathLib {
    struct ETM : Cipher {
        ETM(const Cipher& cipher, const OneWayCipher& hash);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;

        private:
        const Cipher& cipher;
        const OneWayCipher& hash;
    };
}

#endif