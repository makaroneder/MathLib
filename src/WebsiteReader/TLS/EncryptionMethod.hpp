#ifndef EncryptionMethod_H
#define EncryptionMethod_H
#include "TLSCompressionMethod.hpp"
#include "TLSCipherSuite.hpp"
#include <Cryptography/Cipher.hpp>

struct EncryptionMethod : MathLib::Cipher {
    MathLib::Array<uint8_t> macKey;
    MathLib::Array<uint8_t> cipherKey;
    MathLib::Array<uint8_t> cipherIV;
    TLSCipherSuite cipherSuite;
    TLSCompressionMethod compressionMethod;

    EncryptionMethod(void);
    EncryptionMethod(TLSCipherSuite cipherSuite, TLSCompressionMethod compressionMethod);
    [[nodiscard]] virtual MathLib::Array<uint8_t> Encrypt(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key) const override;
    [[nodiscard]] virtual MathLib::Array<uint8_t> DecryptPartial(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key, const MathLib::Interval<size_t>& range) const override;
    [[nodiscard]] bool IsNone(void) const;
    [[nodiscard]] uint8_t GetKeySize(void) const;
    [[nodiscard]] uint8_t GetMACSize(void) const;
    [[nodiscard]] uint8_t GetIVSize(void) const;
    [[nodiscard]] uint8_t GetBlockSize(void) const;
    [[nodiscard]] bool IsSHA1(void) const;
    [[nodiscard]] bool IsSHA256(void) const;
};

#endif