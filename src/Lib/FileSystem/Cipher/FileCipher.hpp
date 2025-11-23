#ifndef MathLib_FileSystem_Cipher_FileCipher_H
#define MathLib_FileSystem_Cipher_FileCipher_H
#include "../../Cryptography/ReadableCipher.hpp"
#include "../../Interfaces/ByteDevice.hpp"
#include "FileCipherData.hpp"

namespace MathLib {
    struct FileCipher : ReadableCipher {
        [[nodiscard]] virtual bool IsValidHeader(Readable& device, const Sequence<uint64_t>& key) const = 0;
        [[nodiscard]] virtual bool IsValidFooter(Readable& device, const Sequence<uint64_t>& key) const = 0;
        [[nodiscard]] virtual bool Create(Writable& device, const Sequence<uint64_t>& key) const = 0;
        [[nodiscard]] virtual bool UpdateHeaderAndFooter(ByteDevice& device, const Sequence<uint64_t>& key) const = 0;
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const Sequence<uint64_t>& key) const = 0;
    };
}

#endif