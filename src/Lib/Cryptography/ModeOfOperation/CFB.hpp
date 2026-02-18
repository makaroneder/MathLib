#ifndef MathLib_Cryptography_ModeOfOperation_CFB_H
#define MathLib_Cryptography_ModeOfOperation_CFB_H
#include "../Cipher.hpp"

namespace MathLib {
    struct CFB : Cipher {
        CFB(Cipher& cipher);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;

        private:
        Cipher& cipher;
    };
}

#endif