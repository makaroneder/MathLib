#ifndef MathLib_FileSystem_Cipher_TAR_H
#define MathLib_FileSystem_Cipher_TAR_H
#include "../IdentityFileCipher.hpp"

namespace MathLib {
    struct TAR : IdentityFileCipher {
        [[nodiscard]] virtual bool IsValidHeader(Readable& device, const CipherKey& key) const override;
        [[nodiscard]] virtual bool IsValidFooter(Readable& device, const CipherKey& key) const override;
        [[nodiscard]] virtual bool Create(Writable& device, const CipherKey& key) const override;
        [[nodiscard]] virtual bool UpdateHeaderAndFooter(ByteDevice& device, const CipherKey& key) const override;
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const CipherKey& key, size_t& unpaddedSize) const override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif