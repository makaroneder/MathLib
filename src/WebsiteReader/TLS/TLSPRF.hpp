#ifndef TLSPRF_H
#define TLSPRF_H
#include <Cryptography/OneWayCipher/HMAC.hpp>

struct TLSPRF : MathLib::OneWayCipher {
    TLSPRF(const MathLib::HMAC& hmac, MathLib::HMAC::BlockSize blockSize, const MathLib::CipherKey& hashKey);
    [[nodiscard]] virtual MathLib::Array<uint8_t> Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key) const override;

    private:
    MathLib::CipherKey hashKey;
    const MathLib::HMAC& hmac;
    MathLib::HMAC::BlockSize blockSize;
};

#endif