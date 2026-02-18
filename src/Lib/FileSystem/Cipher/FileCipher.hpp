#ifndef MathLib_FileSystem_Cipher_FileCipher_H
#define MathLib_FileSystem_Cipher_FileCipher_H
#include "../../Cryptography/ReadableCipher.hpp"
#include "../../Interfaces/ByteDevice.hpp"
#include "FileCipherData.hpp"

namespace MathLib {
    struct FileCipher : ReadableCipher {
        MathLib::String path;

        [[nodiscard]] virtual bool IsValidHeader(Readable& device, const CipherKey& key) const = 0;
        [[nodiscard]] virtual bool IsValidFooter(Readable& device, const CipherKey& key) const = 0;
        [[nodiscard]] virtual bool Create(Writable& device, const CipherKey& key) const = 0;
        [[nodiscard]] virtual bool UpdateHeaderAndFooter(ByteDevice& device, const CipherKey& key) const = 0;
        [[nodiscard]] virtual FileCipherData Identify(Readable& device, const CipherKey& key) const = 0;
    };
}

#endif