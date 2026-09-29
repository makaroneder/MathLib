#include "TLS.hpp"
#include "TLSPRF.hpp"
#include "TLSAlert.hpp"
#include "TLSExtension.hpp"
#include "../ExternArray.hpp"
#include "TLSRecordHeader.hpp"
#include "TLSServerNameType.hpp"
#include "TLSHandshakeHeader.hpp"
#include "../BigInt/NaturalNumber.hpp"
#include "../Interfaces/SavedRWDevice.hpp"
#include "../Interfaces/Sequence/ByteArray.hpp"
#include "../Cryptography/OneWayCipher/HMAC.hpp"
#include "../Cryptography/OneWayCipher/SHA/SHA256.hpp"

namespace MathLib {
    TLSSignatureAndHashAlgorithm TLS::supportedSignatureAndHashAlgorithms[] = {
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA256, TLSSignatureAlgorithm::RSA),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA256, TLSSignatureAlgorithm::DSA),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA256, TLSSignatureAlgorithm::Anonymous),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA224, TLSSignatureAlgorithm::RSA),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA224, TLSSignatureAlgorithm::DSA),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA224, TLSSignatureAlgorithm::Anonymous),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA1, TLSSignatureAlgorithm::RSA),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA1, TLSSignatureAlgorithm::DSA),
        TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA1, TLSSignatureAlgorithm::Anonymous),
    };
    TLS::TLS(RWDevice& base) : dataBuffer(), base(base), writeEncryption(), readEncryption(), writeSequenceNumber(0), readSequenceNumber(0), mode(TLSRecordHeader::Type::Handshake) {}
    bool TLS::PerformHandshake(const Collection<char>& host) {
        TLSRandom clientRandom;
        for (uint8_t i = 0; i < SizeOfArray(clientRandom.random); i++) clientRandom.random[i] = RandomNumber<uint8_t>(0, UINT8_MAX);
        ByteArray handshakeData;
        const bool signatureAlgorithmsExtension = true;
        const bool renegotiationInfoExtension = true;
        const uint16_t hostSize = host.GetSize();
        uint16_t extensionsSize = (
            signatureAlgorithmsExtension * (sizeof(TLSExtension) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(supportedSignatureAndHashAlgorithms)) +
            renegotiationInfoExtension * (sizeof(TLSExtension) + sizeof(uint16_t) + sizeof(uint8_t)) +
            (hostSize ? sizeof(TLSExtension) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(TLSServerNameType) + sizeof(uint16_t) + hostSize : 0) +
            0
        );
        const uint32_t handshakeSize = sizeof(uint16_t) + sizeof(TLSRandom) + sizeof(uint8_t) + sizeof(uint16_t) + sizeof(supportedCipherSuites) + sizeof(uint8_t) + sizeof(supportedCompressionMethods) + sizeof(uint16_t) + extensionsSize;
        if (!handshakeData.Write<TLSHandshakeHeader>(TLSHandshakeHeader(TLSHandshakeHeader::Type::ClientHello, handshakeSize))) return false;
        if (!handshakeData.WriteBigEndian16(version)) return false;
        if (!handshakeData.Write<TLSRandom>(clientRandom)) return false;
        if (!handshakeData.Write<uint8_t>(0)) return false;
        if (!handshakeData.WriteBigEndian16(sizeof(supportedCipherSuites))) return false;
        if (!handshakeData.WriteBigEndianBuffer16(supportedCipherSuites, SizeOfArray(supportedCipherSuites))) return false;
        if (!handshakeData.WriteSizeAndBuffer<uint8_t>(supportedCompressionMethods, sizeof(supportedCompressionMethods))) return false;
        if (!handshakeData.WriteBigEndian16(extensionsSize)) return false;
        if (signatureAlgorithmsExtension) {
            if (!handshakeData.WriteBigEndian16((uint16_t)TLSExtension::SignatureAlgorithms)) return false;
            if (!handshakeData.WriteBigEndian16(sizeof(supportedSignatureAndHashAlgorithms) + sizeof(uint16_t))) return false;
            if (!handshakeData.WriteBigEndian16(sizeof(supportedSignatureAndHashAlgorithms))) return false;
            if (!handshakeData.WriteBuffer(supportedSignatureAndHashAlgorithms, sizeof(supportedSignatureAndHashAlgorithms))) return false;
        }
        if (renegotiationInfoExtension) {
            if (!handshakeData.WriteBigEndian16((uint16_t)TLSExtension::RenegotiationInfo)) return false;
            if (!handshakeData.WriteBigEndian16(1)) return false;
            if (!handshakeData.Write<uint8_t>(0x00)) return false;
        }
        if (hostSize) {
            if (!handshakeData.WriteBigEndian16((uint16_t)TLSExtension::ServerName)) return false;
            if (!handshakeData.WriteBigEndian16(sizeof(uint16_t) + sizeof(TLSServerNameType) + sizeof(uint16_t) + hostSize)) return false;
            if (!handshakeData.WriteBigEndian16(sizeof(TLSServerNameType) + sizeof(uint16_t) + hostSize)) return false;
            if (!handshakeData.Write<TLSServerNameType>(TLSServerNameType::Host)) return false;
            if (!handshakeData.WriteBigEndian16(hostSize)) return false;
            if (!handshakeData.WriteCollection<char>(host)) return false;
        }
        if (!WriteCollection<uint8_t>(handshakeData)) return false;
        SavedRWDevice device = SavedRWDevice(*this, handshakeData);

        TLSHandshakeHeader header;
        if (!device.Read<TLSHandshakeHeader>(header)) return false;
        if (header.type != TLSHandshakeHeader::Type::ServerHello) return false;
        uint16_t serverVersion = 0;
        if (!device.ReadBigEndian16<uint16_t>(serverVersion)) return false;
        TLSRandom serverRandom;
        if (!device.Read<TLSRandom>(serverRandom)) return false;
        uint8_t sessionIDSize = 0;
        if (!device.Read<uint8_t>(sessionIDSize) || sessionIDSize > 32) return false;
        uint8_t sessionID[sessionIDSize];
        if (!device.ReadBuffer(sessionID, sessionIDSize)) return false;
        TLSCipherSuite nextCipherSuite;
        if (!device.ReadBigEndian16<TLSCipherSuite>(nextCipherSuite)) return false;
        TLSCompressionMethod nextCompressionMethod;
        if (!device.Read<TLSCompressionMethod>(nextCompressionMethod)) return false;
        if (!device.ReadBigEndian16<uint16_t>(extensionsSize)) return false;
        Array<uint8_t> extensions = extensionsSize;
        if (!device.ReadCollection<uint8_t>(extensions)) return false;

        MathLib::Array<uint8_t> preMasterSecret;
        MathLib::Array<uint8_t> keyExchange;
        bool sendCertificate = false;
        while (true) {
            if (!device.Read<TLSHandshakeHeader>(header)) return false;
            if (header.type == TLSHandshakeHeader::Type::ServerHelloDone) break;
            switch (header.type) {
                case TLSHandshakeHeader::Type::Certificate: {
                    // TODO: Check if we should receive this
                    Int24 size24;
                    if (!device.Read<Int24>(size24)) return false;
                    uint32_t size = size24.Get();
                    if (header.size.Get() != size + sizeof(Int24)) return false;
                    while (size) {
                        if (!device.Read<Int24>(size24)) return false;
                        uint32_t certificateSize = size24.Get();
                        if (size < certificateSize) return false;
                        // TODO: Verify certificate
                        if (!device.Skip(certificateSize)) return false;
                        size -= certificateSize + sizeof(Int24);
                    }
                    break;
                }
                case TLSHandshakeHeader::Type::ServerKeyExchange: {
                    if (!EncryptionMethod(nextCipherSuite, nextCompressionMethod).GeneratePreMasterSecretAndKeyExchange(device, preMasterSecret, keyExchange)) return false;
                    break;
                }
                case TLSHandshakeHeader::Type::CertificateRequest: {
                    uint8_t certificateTypes = 0;
                    if (!device.Read<uint8_t>(certificateTypes)) return false;
                    if (!device.Skip(certificateTypes)) return false;
                    uint16_t signatureAlgorithms = 0;
                    if (!device.Read<uint16_t>(signatureAlgorithms)) return false;
                    if (!device.Skip(signatureAlgorithms)) return false;
                    uint16_t certificateAuthorities = 0;
                    if (!device.Read<uint16_t>(certificateAuthorities)) return false;
                    if (!device.Skip(certificateAuthorities)) return false;
                    sendCertificate = true;
                    break;
                }
                default: return false;
            }
        }
        if (sendCertificate) {
            if (!device.Write<TLSHandshakeHeader>(TLSHandshakeHeader(TLSHandshakeHeader::Type::Certificate, sizeof(Int24)))) return false;
            if (!device.Write<Int24>(0)) return false;
        }
        Array<uint8_t> randomCS = 32 * 2;
        Array<uint8_t> randomSC = 32 * 2;
        for (uint8_t i = 0; i < 32; i++)
            randomCS.AtUnsafe(i) = randomSC.AtUnsafe(i + 32) = clientRandom.random[i];
        for (uint8_t i = 0; i < 32; i++)
            randomCS.AtUnsafe(i + 32) = randomSC.AtUnsafe(i) = serverRandom.random[i];
        SHA256 hash;
        HMAC hmac = hash;
        TLSPRF prf = TLSPRF(hmac, HMAC::BlockSize::SHA256, CipherKey());
        const Array<uint8_t> masterSecret = prf.EncryptT<char>("master secret"_M, CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(48)),
            CipherKey(preMasterSecret),
            CipherKey(randomCS)
        )));
        const size_t macSize = EncryptionMethod(nextCipherSuite, nextCompressionMethod).GetMACSize();
        const size_t keySize = EncryptionMethod(nextCipherSuite, nextCompressionMethod).GetKeySize();
        const size_t ivSize = EncryptionMethod(nextCipherSuite, nextCompressionMethod).GetIVSize();
        ByteArray keys = prf.EncryptT<char>("key expansion"_M, CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>((macSize + keySize + ivSize) * 2)),
            CipherKey(masterSecret),
            CipherKey(randomSC)
        )));
        writeEncryption.macKey = Array<uint8_t>(macSize);
        if (!keys.ReadCollection<uint8_t>(writeEncryption.macKey)) return false;
        readEncryption.macKey = Array<uint8_t>(macSize);
        if (!keys.ReadCollection<uint8_t>(readEncryption.macKey)) return false;
        writeEncryption.cipherKey = Array<uint8_t>(keySize);
        if (!keys.ReadCollection<uint8_t>(writeEncryption.cipherKey)) return false;
        readEncryption.cipherKey = Array<uint8_t>(keySize);
        if (!keys.ReadCollection<uint8_t>(readEncryption.cipherKey)) return false;
        writeEncryption.cipherIV = Array<uint8_t>(ivSize);
        if (!keys.ReadCollection<uint8_t>(writeEncryption.cipherIV)) return false;
        readEncryption.cipherIV = Array<uint8_t>(ivSize);
        if (!keys.ReadCollection<uint8_t>(readEncryption.cipherIV)) return false;

        if (!device.Write<TLSHandshakeHeader>(TLSHandshakeHeader(TLSHandshakeHeader::Type::ClientKeyExchange, keyExchange.GetSize() + sizeof(uint16_t)))) return false;
        if (!device.WriteBigEndian16(keyExchange.GetSize())) return false;
        if (!device.WriteCollection<uint8_t>(keyExchange)) return false;

        mode = TLSRecordHeader::Type::ChangeCipherSpec;
        if (!Write<uint8_t>(0x01)) return false;
        mode = TLSRecordHeader::Type::Handshake;
        writeEncryption.cipherSuite = nextCipherSuite;
        writeEncryption.compressionMethod = nextCompressionMethod;
        writeSequenceNumber = 0;

        const Array<uint8_t> clientFinished = prf.EncryptT<char>("client finished"_M, CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(12)),
            CipherKey(masterSecret),
            CipherKey(SHA256().Encrypt(handshakeData, CipherKey()))
        )));
        if (!device.Write<TLSHandshakeHeader>(TLSHandshakeHeader(TLSHandshakeHeader::Type::Finished, clientFinished.GetSize()))) return false;
        if (!device.WriteCollection<uint8_t>(clientFinished)) return false;
        mode = TLSRecordHeader::Type::ChangeCipherSpec;
        if (Read<uint8_t>().GetOr(0) != 0x01) return false;
        mode = TLSRecordHeader::Type::Handshake;
        readEncryption.cipherSuite = nextCipherSuite;
        readEncryption.compressionMethod = nextCompressionMethod;
        readSequenceNumber = 0;
        if (!Read<TLSHandshakeHeader>(header)) return false;
        if (header.type != TLSHandshakeHeader::Type::Finished) return false;
        if (header.size.Get() != 12) return false;
        Array<uint8_t> serverFinished = 12;
        if (!ReadCollection<uint8_t>(serverFinished)) return false;
        if (serverFinished != prf.EncryptT<char>("server finished"_M, CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(12)),
            CipherKey(masterSecret),
            CipherKey(SHA256().Encrypt(handshakeData, CipherKey()))
        )))) return false;
        mode = TLSRecordHeader::Type::ApplicationData;
        return true;
    }
    size_t TLS::ReadSizedBuffer(void* buffer, size_t size) {
        if (!size) return 0;
        const size_t dataBufferSize = dataBuffer.GetSize();
        uint8_t* const buffer8 = (uint8_t*)buffer;
        if (!dataBufferSize) {
            TLSRecordHeader record;
            while (true) {
                if (!base.Read<TLSRecordHeader>(record)) return 0;
                record.size = SwapEndian16(record.size);
                if (record.type == mode) break;
                if (record.type != TLSRecordHeader::Type::Alert) return 0;
                TLSAlert alert;
                if (!base.Read<TLSAlert>(alert)) return 0;
                if (alert.level == TLSAlert::Level::Fatal || !base.Skip(record.size - sizeof(TLSAlert))) return 0;
                break;
            }
            dataBuffer = record.size;
            if (!base.ReadCollection<uint8_t>(dataBuffer)) return 0;
            dataBuffer = readEncryption.Decrypt(dataBuffer, CipherKey(MakeArray<CipherKey>(
                CipherKey(ByteArray::ToByteArray<TLSRecordHeader::Type>(mode)),
                CipherKey(ByteArray::ToByteArray<uint16_t>(record.version)),
                CipherKey(ByteArray::ToByteArray<uint64_t>(readSequenceNumber++))
            )));
            return ReadSizedBuffer(buffer, size);
        }
        if (dataBufferSize <= size) {
            for (size_t i = 0; i < dataBufferSize; i++) buffer8[i] = dataBuffer.AtUnsafe(i);
            dataBuffer = Array<uint8_t>();
            return dataBufferSize + ReadSizedBuffer(buffer8 + dataBufferSize, size - dataBufferSize);
        }
        for (size_t i = 0; i < size; i++) buffer8[i] = dataBuffer.AtUnsafe(i);
        Array<uint8_t> tmp = dataBufferSize - size;
        for (size_t i = size; i < dataBufferSize; i++) tmp.AtUnsafe(i - size) = dataBuffer.AtUnsafe(i);
        dataBuffer = tmp;
        return size;
    }
    size_t TLS::WriteSizedBuffer(const void* buffer, size_t size) {
        const uint16_t limit = 1 << 14;
        size_t position = 0;
        while (size) {
            const uint16_t curr = size <= limit ? size : limit;
            const Array<uint8_t> data = writeEncryption.Encrypt(ExternArray<uint8_t>((uint8_t*)buffer + position, curr), CipherKey(MakeArray<CipherKey>(
                CipherKey(ByteArray::ToByteArray<TLSRecordHeader::Type>(mode)),
                CipherKey(ByteArray::ToByteArray<uint16_t>(version)),
                CipherKey(ByteArray::ToByteArray<uint64_t>(writeSequenceNumber++))
            )));
            if (!base.Write<TLSRecordHeader>(TLSRecordHeader(mode, SwapEndian16(version), SwapEndian16(data.GetSize())))) break;
            if (!base.WriteCollection<uint8_t>(data)) break;
            position += curr;
            size -= curr;
        }
        return position;
    }
    bool TLS::Skip(size_t size) {
        return DefaultSkip(size);
    }
}