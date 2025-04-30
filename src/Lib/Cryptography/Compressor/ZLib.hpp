#ifndef MathLib_Cryptography_Compressor_ZLib_H
#define MathLib_Cryptography_Compressor_ZLib_H
#include "../ReadableCipher.hpp"

namespace MathLib {
    struct ZLib : ReadableCipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const Sequence<uint64_t>& key, const Interval<size_t>& range) const override;
    };
}

#endif