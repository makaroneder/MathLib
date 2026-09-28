#ifndef TLS_EncryptionMethod_H
#define TLS_EncryptionMethod_H
#include "../Cryptography/OneWayCipher/HMAC.hpp"
#include "../Cryptography/Cipher.hpp"
#include "TLSCompressionMethod.hpp"
#include "TLSCipherSuite.hpp"

namespace MathLib {
    struct EncryptionMethod : Cipher {
        Array<uint8_t> macKey;
        Array<uint8_t> cipherKey;
        Array<uint8_t> cipherIV;
        TLSCipherSuite cipherSuite;
        TLSCompressionMethod compressionMethod;

        EncryptionMethod(void);
        EncryptionMethod(TLSCipherSuite cipherSuite, TLSCompressionMethod compressionMethod);
        EncryptionMethod(TLSCipherSuite cipherSuite, TLSCompressionMethod compressionMethod, const Array<uint8_t>& macKey, const Array<uint8_t>& cipherKey, const Array<uint8_t>& cipherIV);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const override;
        [[nodiscard]] bool IsNone(void) const;
        [[nodiscard]] uint8_t GetKeySize(void) const;
        [[nodiscard]] uint8_t GetMACSize(void) const;
        [[nodiscard]] uint8_t GetIVSize(void) const;
        [[nodiscard]] uint8_t GetBlockSize(void) const;
        [[nodiscard]] bool IsSHA1(void) const;
        [[nodiscard]] bool IsSHA256(void) const;
        [[nodiscard]] Cipher* GetCipher(Cipher*& tmpCipher, CipherKey& key) const;
        [[nodiscard]] OneWayCipher* GetHash(HMAC::BlockSize& blockSize, CipherKey& key) const;
    };
}

#endif