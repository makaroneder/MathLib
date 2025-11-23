#ifndef MathLib_FileSystem_Cipher_TAR_UnixStandard_H
#define MathLib_FileSystem_Cipher_TAR_UnixStandard_H
#include "../TAR.hpp"

namespace MathLib {
    struct UnixStandardTAR : TAR {
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const Sequence<uint64_t>& key, size_t& unpaddedSize) const override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
    };
}

#endif