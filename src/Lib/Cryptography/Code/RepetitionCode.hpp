#ifndef MathLib_Cryptography_Code_RepetitionCode_H
#define MathLib_Cryptography_Code_RepetitionCode_H
#include "../Cipher.hpp"

namespace MathLib {
    struct RepetitionCode : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;
    };
}

#endif