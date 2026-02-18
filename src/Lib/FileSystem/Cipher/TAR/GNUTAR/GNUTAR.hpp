#ifndef MathLib_FileSystem_Cipher_TAR_GNU_H
#define MathLib_FileSystem_Cipher_TAR_GNU_H
#include "../TAR.hpp"

namespace MathLib {
    struct GNUTAR : TAR {
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const CipherKey& key, size_t& unpaddedSize) const override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
    };
}

#endif