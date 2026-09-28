#include "../Cryptography/OneWayCipher/SHA/SHA256.hpp"
#include "../Cryptography/OneWayCipher/SHA/SHA1.hpp"
#include "../Cryptography/ModeOfOperation/CBC.hpp"
#include "../Interfaces/Sequence/SubSequence.hpp"
#include "../Interfaces/Sequence/ByteArray.hpp"
#include "../Cryptography/Cipher/AES.hpp"
#include "EncryptionMethod.hpp"
#include "TLSRecordHeader.hpp"

namespace MathLib {
    EncryptionMethod::EncryptionMethod(void) : macKey(), cipherKey(), cipherIV(), cipherSuite(TLSCipherSuite::None), compressionMethod(TLSCompressionMethod::None) {}
    EncryptionMethod::EncryptionMethod(TLSCipherSuite cipherSuite, TLSCompressionMethod compressionMethod) : macKey(), cipherKey(), cipherIV(), cipherSuite(cipherSuite), compressionMethod(compressionMethod) {}
    EncryptionMethod::EncryptionMethod(TLSCipherSuite cipherSuite, TLSCompressionMethod compressionMethod, const Array<uint8_t>& macKey, const Array<uint8_t>& cipherKey, const Array<uint8_t>& cipherIV) : macKey(macKey), cipherKey(cipherKey), cipherIV(cipherIV), cipherSuite(cipherSuite), compressionMethod(compressionMethod) {}
    Cipher* EncryptionMethod::GetCipher(Cipher*& tmpCipher, CipherKey& key) const {
        switch (cipherSuite) {
            case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
            case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
            case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA:
            case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: {
                AES* const tmp = new AES();
                tmpCipher = tmp;
                if (!tmpCipher) return nullptr;
                AES::Rounds rounds;
                switch (GetKeySize()) {
                    case 128 / 8: {
                        rounds = AES::Rounds::AES128;
                        break;
                    }
                    case 256 / 8: {
                        rounds = AES::Rounds::AES256;
                        break;
                    }
                    default: {
                        delete tmpCipher;
                        return nullptr;
                    }
                }
                key = tmp->GenerateKey(CipherKey(MakeArray<CipherKey>(
                    CipherKey(ByteArray::ToByteArray<AES::Rounds>(rounds)),
                    CipherKey(cipherKey)
                )));
                CBC* const ret = new CBC(*tmpCipher);
                if (ret) return ret;
                delete tmpCipher;
                return nullptr;
            }
            default: return nullptr;
        }
    }
    OneWayCipher* EncryptionMethod::GetHash(HMAC::BlockSize& blockSize, CipherKey& key) const {
        switch (cipherSuite) {
            case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA:
            case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA: {
                blockSize = HMAC::BlockSize::SHA1;
                key = CipherKey();
                return new SHA1();
            }
            case TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256:
            case TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256: {
                blockSize = HMAC::BlockSize::SHA256;
                key = CipherKey();
                return new SHA256();
            }
            default: return nullptr;
        }
    }
    Array<uint8_t> EncryptionMethod::Encrypt(const Sequence<uint8_t>& data_, const CipherKey& key) const {
        const size_t size = data_.GetSize();
        Array<uint8_t> data = CollectionToArray<uint8_t>(data_);
        if (size > (1 << 14) || !key.CheckChildrenSize(3)) return Array<uint8_t>();
        if (!key.children.AtUnsafe(0).CheckDataSize(sizeof(TLSRecordHeader::Type))) return Array<uint8_t>();
        const TLSRecordHeader::Type type = key.children.AtUnsafe(0).data.AsT<TLSRecordHeader::Type>().Get();
        if (!key.children.AtUnsafe(1).CheckDataSize(sizeof(uint16_t))) return Array<uint8_t>();
        const uint16_t version = SwapEndian16(key.children.AtUnsafe(1).data.AsT<uint16_t>().Get());
        if (!key.children.AtUnsafe(2).CheckDataSize(sizeof(uint64_t))) return Array<uint8_t>();
        const uint64_t sequenceNumber = SwapEndian64(key.children.AtUnsafe(2).data.AsT<uint64_t>().Get());
        if (cipherSuite == TLSCipherSuite::None) return data;
        ByteArray tmp;
        if (!tmp.Write<uint64_t>(sequenceNumber) || !tmp.Write<TLSRecordHeader::Type>(type) || !tmp.Write<uint16_t>(version) || !tmp.WriteBigEndian16(size)) return Array<uint8_t>();
        for (size_t i = 0; i < size; i++)
            if (!tmp.Write<uint8_t>(data.AtUnsafe(i))) return Array<uint8_t>();
        HMAC::BlockSize blockSize;
        CipherKey hashKey;
        OneWayCipher* const hash = GetHash(blockSize, hashKey);
        if (!hash) return Array<uint8_t>();
        if (!data.AddSequence(HMAC(*hash).Encrypt(tmp, CipherKey(MakeArray<CipherKey>(ByteArray::ToByteArray<HMAC::BlockSize>(blockSize), macKey, hashKey))))) {
            delete hash;
            return Array<uint8_t>();
        }
        delete hash;
        const uint8_t cipherBlockSize = GetBlockSize();
        uint8_t padding = 0;
        while ((data.GetSize() + padding + 1) % cipherBlockSize) padding++;
        for (uint16_t i = 0; i <= padding; i++)
            if (!data.Add(padding)) return Array<uint8_t>();
        Cipher* tmpCipher = nullptr;
        CipherKey tmpKey;
        Cipher* const cipher = GetCipher(tmpCipher, tmpKey);
        if (!cipher) {
            if (tmpCipher) delete tmpCipher;
            return Array<uint8_t>();
        }
        Array<uint8_t> ret = GetIVSize();
        for (uint16_t i = 0; i < ret.GetSize(); i++) ret.AtUnsafe(i) = RandomNumber<uint8_t>(0, UINT8_MAX);
        const bool status = ret.AddSequence(cipher->Encrypt(data, CipherKey(MakeArray<CipherKey>(CipherKey(ret), tmpKey))));
        delete cipher;
        if (tmpCipher) delete tmpCipher;
        return status ? ret : Array<uint8_t>();
    }
    Array<uint8_t> EncryptionMethod::DecryptPartial(const Sequence<uint8_t>& data_, const CipherKey& key, const Interval<size_t>& range) const {
        const size_t start = range.GetMin();
        const size_t size = data_.GetSize();
        if (size > (1 << 14) || !key.CheckChildrenSize(3)) return Array<uint8_t>();
        if (!key.children.AtUnsafe(0).CheckDataSize(sizeof(TLSRecordHeader::Type))) return Array<uint8_t>();
        const TLSRecordHeader::Type type = key.children.AtUnsafe(0).data.AsT<TLSRecordHeader::Type>().Get();
        if (!key.children.AtUnsafe(1).CheckDataSize(sizeof(uint16_t))) return Array<uint8_t>();
        const uint16_t version = SwapEndian16(key.children.AtUnsafe(1).data.AsT<uint16_t>().Get());
        if (!key.children.AtUnsafe(2).CheckDataSize(sizeof(uint64_t))) return Array<uint8_t>();
        const uint64_t sequenceNumber = SwapEndian64(key.children.AtUnsafe(2).data.AsT<uint64_t>().Get());
        if (cipherSuite == TLSCipherSuite::None) {
            const size_t end = Min<size_t>(size, range.GetMax());
            if (start >= end) return Array<uint8_t>();
            return CollectionToArray<uint8_t>(SubSequence<uint8_t>(data_, Interval<size_t>(start, end)));
        }
        const SubSequence<uint8_t> iv = SubSequence(data_, Interval<size_t>(0, 16));
        const SubSequence<uint8_t> data = SubSequence(data_, Interval<size_t>(16, size));

        Cipher* tmpCipher = nullptr;
        CipherKey tmpKey;
        Cipher* const cipher = GetCipher(tmpCipher, tmpKey);
        if (!cipher) {
            if (tmpCipher) delete tmpCipher;
            return Array<uint8_t>();
        }
        const Array<uint8_t> decrypted = cipher->Decrypt(data, CipherKey(MakeArray<CipherKey>(CipherKey(iv), tmpKey)));
        if (tmpCipher) delete tmpCipher;
        delete cipher;
        const uint8_t paddingSize = decrypted.At(decrypted.GetSize() - 1);
        for (uint8_t i = 0; i < paddingSize; i++)
            if (decrypted.At(decrypted.GetSize() - 1 - paddingSize + i) != paddingSize) return Array<uint8_t>();
        const uint8_t macSize = GetMACSize();
        const size_t finalSize = decrypted.GetSize() - 1 - paddingSize - macSize;
        const size_t end = Min<size_t>(finalSize, range.GetMax());
        if (start >= end) return Array<uint8_t>();
        ByteArray tmp;
        if (!tmp.Write<uint64_t>(sequenceNumber) || !tmp.Write<TLSRecordHeader::Type>(type) || !tmp.Write<uint16_t>(version) || !tmp.WriteBigEndian16(finalSize)) return Array<uint8_t>();
        for (size_t i = 0; i < finalSize; i++)
            if (!tmp.Write<uint8_t>(decrypted.AtUnsafe(i))) return Array<uint8_t>();
        HMAC::BlockSize blockSize;
        CipherKey hashKey;
        OneWayCipher* const hash = GetHash(blockSize, hashKey);
        if (!hash) return Array<uint8_t>();
        const bool status = SubSequence<uint8_t>(decrypted, Interval<size_t>(finalSize, finalSize + macSize)) == HMAC(*hash).Encrypt(tmp, CipherKey(MakeArray<CipherKey>(ByteArray::ToByteArray<HMAC::BlockSize>(blockSize), macKey, hashKey)));
        delete hash;
        return status ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(decrypted, Interval<size_t>(start, end))) : Array<uint8_t>();
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
}