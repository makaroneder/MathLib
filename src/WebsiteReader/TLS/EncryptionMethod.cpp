#include "EncryptionMethod.hpp"
#include "TLSRecordHeader.hpp"
#include <Cryptography/Cipher/AES.hpp>
#include <Interfaces/Sequence/ByteArray.hpp>
#include <Cryptography/OneWayCipher/HMAC.hpp>
#include <Cryptography/ModeOfOperation/CBC.hpp>
#include <Cryptography/OneWayCipher/SHA/SHA1.hpp>
#include <Cryptography/OneWayCipher/SHA/SHA256.hpp>

EncryptionMethod::EncryptionMethod(void) : cipherSuite(TLSCipherSuite::None), compressionMethod(TLSCompressionMethod::None) {}
EncryptionMethod::EncryptionMethod(TLSCipherSuite cipherSuite, TLSCompressionMethod compressionMethod) : cipherSuite(cipherSuite), compressionMethod(compressionMethod) {}
MathLib::Array<uint8_t> EncryptionMethod::Encrypt(const MathLib::Sequence<uint8_t>& data_, const MathLib::CipherKey& key) const {
    const size_t size = data_.GetSize();
    MathLib::Array<uint8_t> data = MathLib::CollectionToArray<uint8_t>(data_);
    if (size > UINT16_MAX || !key.CheckChildrenSize(3)) return MathLib::Array<uint8_t>();
    if (!key.children.AtUnsafe(0).CheckDataSize(sizeof(TLSRecordHeader::Type))) return MathLib::Array<uint8_t>();
    const TLSRecordHeader::Type type = key.children.AtUnsafe(0).data.AsT<TLSRecordHeader::Type>().Get();
    if (!key.children.AtUnsafe(1).CheckDataSize(sizeof(uint16_t))) return MathLib::Array<uint8_t>();
    const uint16_t version = MathLib::SwapEndian16(key.children.AtUnsafe(1).data.AsT<uint16_t>().Get());
    if (!key.children.AtUnsafe(2).CheckDataSize(sizeof(uint64_t))) return MathLib::Array<uint8_t>();
    const uint64_t sequenceNumber = MathLib::SwapEndian64(key.children.AtUnsafe(2).data.AsT<uint64_t>().Get());
    if (cipherSuite == TLSCipherSuite::None) return data;
    const MathLib::OneWayCipher* hash = nullptr;
    MathLib::HMAC::BlockSize blockSize;
    MathLib::CipherKey hashKey;
    switch (cipherSuite) {
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA: {
            hash = new MathLib::SHA1();
            blockSize = MathLib::HMAC::BlockSize::SHA1;
            break;
        }
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: {
            hash = new MathLib::SHA256();
            blockSize = MathLib::HMAC::BlockSize::SHA256;
            break;
        }
        default: return MathLib::Array<uint8_t>();
    }
    if (!hash) return MathLib::Array<uint8_t>();
    MathLib::HMAC hmac = *hash;
    MathLib::ByteArray tmp;
    if (!tmp.Write<uint64_t>(sequenceNumber)) return MathLib::Array<uint8_t>();
    if (!tmp.Write<TLSRecordHeader::Type>(type)) return MathLib::Array<uint8_t>();
    if (!tmp.Write<uint16_t>(version)) return MathLib::Array<uint8_t>();
    if (!tmp.Write<uint16_t>(size)) return MathLib::Array<uint8_t>();
    for (size_t i = 0; i < size; i++)
        if (!tmp.Write<uint8_t>(data.AtUnsafe(i))) return MathLib::Array<uint8_t>();
    if (!data.AddSequence(hmac.Encrypt(tmp, MathLib::CipherKey(MathLib::MakeArray<MathLib::CipherKey>(MathLib::ByteArray::ToByteArray<MathLib::HMAC::BlockSize>(blockSize), macKey, hashKey))))) return MathLib::Array<uint8_t>();
    delete hash;
    const uint8_t cipherBlockSize = GetBlockSize();
    uint8_t padding = 0;
    while ((data.GetSize() + padding + 1) % cipherBlockSize) padding++;
    for (uint16_t i = 0; i <= padding; i++)
        if (!data.Add(padding)) return MathLib::Array<uint8_t>();
    const MathLib::Cipher* tmpCipher = nullptr;
    const MathLib::Cipher* cipher = nullptr;
    MathLib::CipherKey key;
    switch (cipherSuite) {
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: {
            MathLib::AES* tmp = new MathLib::AES();
            if (!tmp) return MathLib::Array<uint8_t>();
            tmpCipher = tmp;
            MathLib::AES::Rounds rounds;
            switch (GetKeySize()) {
                case 128 / 8: {
                    rounds = MathLib::AES::Rounds::AES128;
                    break;
                }
                case 256 / 8: {
                    rounds = MathLib::AES::Rounds::AES256;
                    break;
                }
                default: return MathLib::Array<uint8_t>();
            }
            key = tmp->GenerateKey(MathLib::CipherKey(MathLib::MakeArray<MathLib::CipherKey>(
                MathLib::CipherKey(MathLib::ByteArray::ToByteArray<MathLib::AES::Rounds>(rounds)),
                MathLib::CipherKey(cipherKey)
            )));
            cipher = new MathLib::CBC(*tmpCipher);
            break;
        }
        default: return MathLib::Array<uint8_t>();
    }
    if (!cipher) {
        if (tmpCipher) delete tmpCipher;
        return MathLib::Array<uint8_t>();
    }
    MathLib::Array<uint8_t> ret = cipherIV;
    if (!ret.AddSequence(cipher->Encrypt(data, MathLib::CipherKey(MathLib::MakeArray<MathLib::CipherKey>(MathLib::CipherKey(ret), key))))) return MathLib::Array<uint8_t>();
    delete cipher;
    if (tmpCipher) delete tmpCipher;
    return ret;
}
MathLib::Array<uint8_t> EncryptionMethod::DecryptPartial(const MathLib::Sequence<uint8_t>& data, const MathLib::CipherKey& key, const MathLib::Interval<size_t>& range) const {
    // TODO:
    (void)data;
    (void)key;
    (void)range;
    return MathLib::Array<uint8_t>();
}
bool EncryptionMethod::IsNone(void) const {
    return compressionMethod == TLSCompressionMethod::None && cipherSuite == TLSCipherSuite::None;
}
uint8_t EncryptionMethod::GetKeySize(void) const {
    switch (cipherSuite) {
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256: return 128 / 8;
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: return 256 / 8;
        default: return 0;
    }
}
uint8_t EncryptionMethod::GetMACSize(void) const {
    switch (cipherSuite) {
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA: return 20;
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: return 32;
        default: return 0;
    }
}
uint8_t EncryptionMethod::GetIVSize(void) const {
    switch (cipherSuite) {
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: return 16;
        default: return 0;
    }
}
uint8_t EncryptionMethod::GetBlockSize(void) const {
    switch (cipherSuite) {
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA:
        case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: return 16;
        default: return 0;
    }
}
bool EncryptionMethod::IsSHA1(void) const {
    return cipherSuite == TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA || cipherSuite == TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA;
}
bool EncryptionMethod::IsSHA256(void) const {
    return cipherSuite == TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256 || cipherSuite == TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256;
}