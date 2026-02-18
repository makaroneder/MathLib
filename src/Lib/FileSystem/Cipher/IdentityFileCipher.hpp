#ifndef MathLib_FileSystem_Cipher_IdentityFileCipher_H
#define MathLib_FileSystem_Cipher_IdentityFileCipher_H
#include "FileCipher.hpp"

namespace MathLib {
    struct IdentityFileCipher : FileCipher {
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const CipherKey& key, size_t& unpaddedSize) const = 0;
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const override;
    };
}

#endif