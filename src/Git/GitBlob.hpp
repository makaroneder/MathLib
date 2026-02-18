#ifndef GitBlob_H
#define GitBlob_H
#include <Cryptography/ReadableCipher.hpp>

struct GitBlob : MathLib::ReadableCipher {
    static constexpr const char* blobStart = "blob ";

    [[nodiscard]] virtual MathLib::Array<uint8_t> Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key) const override;
    [[nodiscard]] virtual MathLib::Array<uint8_t> DecryptReadablePartial(MathLib::Readable& readable, const MathLib::CipherKey& key, const MathLib::Interval<size_t>& range) const override;
};

#endif