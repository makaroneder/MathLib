#ifndef TLS_PRF_H
#define TLS_PRF_H
#include "../Cryptography/OneWayCipher/HMAC.hpp"

namespace MathLib {
    struct TLSPRF : OneWayCipher {
        TLSPRF(const HMAC& hmac, HMAC::BlockSize blockSize, const CipherKey& hashKey);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;

        private:
        CipherKey hashKey;
        const HMAC& hmac;
        HMAC::BlockSize blockSize;
    };
}

#endif