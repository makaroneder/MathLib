#ifndef MathLib_FileSystem_Cipher_Ar_H
#define MathLib_FileSystem_Cipher_Ar_H
#include "../IdentityFileCipher.hpp"

namespace MathLib {
    struct Ar : IdentityFileCipher {
        static constexpr const char* signature1 = "!<arch>\n";
        static constexpr const char* signature2 = "!<thin>\n";

        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual bool IsValidHeader(Readable& readable, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual bool IsValidFooter(Readable& readable, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual bool Create(Writable& writable, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual bool UpdateHeaderAndFooter(ByteDevice& device, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual FileCipherData Identify(Readable& readable, const Sequence<uint64_t>& key, size_t& unpaddedSize) const override;
    };
}

#endif