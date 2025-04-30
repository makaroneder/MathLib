#ifndef MathLib_Cryptography_ReadableCipher_H
#define MathLib_Cryptography_ReadableCipher_H
#include "Cipher.hpp"
#include "../Interfaces/Readable.hpp"

namespace MathLib {
    struct ReadableCipher : Cipher {
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const Sequence<uint64_t>& key, const Interval<size_t>& range) const = 0;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const override;
        [[nodiscard]] Array<uint8_t> DecryptReadable(Readable& readable, const Sequence<uint64_t>& key) const;
    };
}

#endif