#ifndef MathLib_Cryptography_Compressor_GZIP_H
#define MathLib_Cryptography_Compressor_GZIP_H
#include "../Cipher.hpp"

namespace MathLib {
    struct GZIP : Cipher {
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override;
    };
}

#endif