#ifndef MathLib_Cryptography_Code_RunLengthEncoding_H
#define MathLib_Cryptography_Code_RunLengthEncoding_H
#include "../Cipher.hpp"

namespace MathLib {
    struct RunLengthEncoding : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const override;
    };
}

#endif