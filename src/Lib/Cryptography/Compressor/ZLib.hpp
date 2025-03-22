#ifndef MathLib_Cryptography_Compressor_ZLib_H
#define MathLib_Cryptography_Compressor_ZLib_H
#include "../Cipher.hpp"

namespace MathLib {
    struct ZLib : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override;
    };
}

#endif