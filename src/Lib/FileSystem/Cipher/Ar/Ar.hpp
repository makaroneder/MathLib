#ifndef MathLib_FileSystem_Cipher_Ar_H
#define MathLib_FileSystem_Cipher_Ar_H
#include "../IdentityFileCipher.hpp"

namespace MathLib {
    struct Ar : IdentityFileCipher {
        static constexpr const char* signature1 = "!<arch>\n";
        static constexpr const char* signature2 = "!<thin>\n";

        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual bool IsValidHeader(Readable& readable, const CipherKey& key) const override;
        [[nodiscard]] virtual bool IsValidFooter(Readable& readable, const CipherKey& key) const override;
        [[nodiscard]] virtual bool Create(Writable& writable, const CipherKey& key) const override;
        [[nodiscard]] virtual bool UpdateHeaderAndFooter(ByteDevice& device, const CipherKey& key) const override;
        [[nodiscard]] virtual FileCipherData Identify(Readable& readable, const CipherKey& key, size_t& unpaddedSize) const override;
    };
}

#endif