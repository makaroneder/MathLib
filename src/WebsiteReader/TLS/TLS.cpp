#include "TLS.hpp"
#include "TLSAlert.hpp"
#include "TLSExtension.hpp"
#include "TLSRecordHeader.hpp"
#include "TLSServerNameType.hpp"
#include "TLSHandshakeHeader.hpp"
#include <Interfaces/Sequence/ByteArray.hpp>
#include <BigInt/NaturalNumber.hpp>

TLSSignatureAndHashAlgorithm TLS::supportedSignatureAndHashAlgorithms[] = {
    TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA256, TLSSignatureAlgorithm::RSA),
    TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA224, TLSSignatureAlgorithm::RSA),
    TLSSignatureAndHashAlgorithm(TLSHashAlgorithm::SHA1, TLSSignatureAlgorithm::RSA),
};
TLS::TLS(MathLib::RWDevice& base) : dataBuffer(), base(base), cipherSuite(TLSCipherSuite::None), compressionMethod(TLSCompressionMethod::None), mode(TLSRecordHeader::Type::Handshake) {}
bool TLS::PerformHandshake(const MathLib::Collection<char>& host) {
    TLSRandom clientRandom;
    for (uint8_t i = 0; i < SizeOfArray(clientRandom.random); i++) clientRandom.random[i] = MathLib::RandomNumber<uint8_t>(0, UINT8_MAX);
    if (!SendClientHello(host, clientRandom)) return false;

    TLSHandshakeHeader header;
    if (!Read<TLSHandshakeHeader>(header)) return false;
    if (header.type != TLSHandshakeHeader::Type::ServerHello) return false;
    uint16_t serverVersion = 0;
    if (!ReadBigEndian16<uint16_t>(serverVersion)) return false;
    TLSRandom serverRandom;
    if (!Read<TLSRandom>(serverRandom)) return false;
    uint8_t sessionIDSize = 0;
    if (!Read<uint8_t>(sessionIDSize) || sessionIDSize > 32) return false;
    uint8_t sessionID[sessionIDSize];
    if (!ReadBuffer(sessionID, sessionIDSize)) return false;
    TLSCipherSuite nextCipherSuite;
    if (!ReadBigEndian16<TLSCipherSuite>(nextCipherSuite)) return false;
    TLSCompressionMethod nextCompressionMethod;
    if (!Read<TLSCompressionMethod>(nextCompressionMethod)) return false;
    uint16_t extensionsSize = 0;
    if (!ReadBigEndian16<uint16_t>(extensionsSize)) return false;
    MathLib::Array<uint8_t> extensions = extensionsSize;
    if (!ReadCollection<uint8_t>(extensions)) return false;

    MathLib::NaturalNumber modulo;
    MathLib::NaturalNumber generator;
    MathLib::NaturalNumber publicKey;
    while (true) {
        TLSHandshakeHeader header;
        if (!Read<TLSHandshakeHeader>(header)) return false;
        if (header.type == TLSHandshakeHeader::Type::ServerHelloDone) break;
        switch (header.type) {
            case TLSHandshakeHeader::Type::Certificate: {
                // TODO: Check if we should receive this
                Int24 size24;
                if (!Read<Int24>(size24)) return false;
                uint32_t size = size24.Get();
                if (header.size.Get() != size + sizeof(Int24)) return false;
                while (size) {
                    if (!Read<Int24>(size24)) return false;
                    uint32_t certificateSize = size24.Get();
                    if (size < certificateSize) return false;
                    // TODO: Verify certificate
                    if (!Skip(certificateSize)) return false;
                    size -= certificateSize + sizeof(Int24);
                }
                break;
            }
            case TLSHandshakeHeader::Type::ServerKeyExchange: {
                // TODO: Check if we should receive this
                uint32_t sizeSum = 0;
                MathLib::Array<uint8_t> tmp[3];
                for (uint8_t i = 0; i < 3; i++) {
                    uint16_t size = 0;
                    if (!ReadBigEndian16(size)) return false;
                    sizeSum += sizeof(uint16_t) + size;
                    tmp[i] = size;
                    if (!ReadCollection<uint8_t>(tmp[i])) return false;
                }
                modulo = tmp[0];
                generator = tmp[1];
                publicKey = tmp[2];
                TLSSignatureAndHashAlgorithm algorithm;
                if (!Read<TLSSignatureAndHashAlgorithm>(algorithm)) return false;
                uint16_t size = 0;
                if (!ReadBigEndian16(size)) return false;
                MathLib::Array<uint8_t> signature = size;
                if (!ReadCollection<uint8_t>(signature)) return false;
                if (header.size.Get() != sizeSum + sizeof(TLSSignatureAndHashAlgorithm) + sizeof(uint16_t) + size) return false;
                // TODO: Verify signature
                break;
            }
            case TLSHandshakeHeader::Type::CertificateRequest: {
                // TODO:
                return false;
            }
            default: return false;
        }
    }
    // TODO: Compute master secret
    // TODO: Send empty or self signing certificate if requested
    const MathLib::NaturalNumber tmp = generator % modulo;
    if (!Write<TLSHandshakeHeader>(TLSHandshakeHeader(TLSHandshakeHeader::Type::ClientKeyExchange, tmp.GetSize() + sizeof(uint16_t)))) return false;
    if (!WriteBigEndian16(tmp.GetSize())) return false;
    if (!WriteCollection<uint8_t>(tmp.data)) return false;

    mode = TLSRecordHeader::Type::ChangeCipherSpec;
    if (!Write<uint8_t>(1)) return false;
    mode = TLSRecordHeader::Type::Handshake;

    // TODO: Send Finished
    // TODO: Recieve ChangeCipherSpec
    // TODO: Recieve Finished

    return true;
}
bool TLS::SendClientHello(const MathLib::Collection<char>& host, const TLSRandom& random) {
    MathLib::ByteArray buffer;
    const bool signatureAlgorithmsExtension = true;
    const bool renegotiationInfoExtension = true;
    const uint16_t hostSize = host.GetSize();
    const uint16_t extensionsSize = (
        signatureAlgorithmsExtension * (sizeof(TLSExtension) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(supportedSignatureAndHashAlgorithms)) +
        renegotiationInfoExtension * (sizeof(TLSExtension) + sizeof(uint16_t) + sizeof(uint8_t)) +
        (hostSize ? sizeof(TLSExtension) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(TLSServerNameType) + sizeof(uint16_t) + hostSize : 0) +
        0
    );
    const uint32_t handshakeSize = sizeof(uint16_t) + sizeof(TLSRandom) + sizeof(uint8_t) + sizeof(uint16_t) + sizeof(supportedCipherSuites) + sizeof(uint8_t) + sizeof(supportedCompressionMethods) + sizeof(uint16_t) + extensionsSize;
    if (!buffer.Write<TLSHandshakeHeader>(TLSHandshakeHeader(TLSHandshakeHeader::Type::ClientHello, handshakeSize))) return false;
    if (!buffer.WriteBigEndian16(version)) return false;
    if (!buffer.Write<TLSRandom>(random)) return false;
    if (!buffer.Write<uint8_t>(0)) return false;
    if (!buffer.WriteBigEndian16(sizeof(supportedCipherSuites))) return false;
    if (!buffer.WriteBigEndianBuffer16(supportedCipherSuites, SizeOfArray(supportedCipherSuites))) return false;
    if (!buffer.WriteSizeAndBuffer<uint8_t>(supportedCompressionMethods, sizeof(supportedCompressionMethods))) return false;
    if (!buffer.WriteBigEndian16(extensionsSize)) return false;
    if (signatureAlgorithmsExtension) {
        if (!buffer.WriteBigEndian16((uint16_t)TLSExtension::SignatureAlgorithms)) return false;
        if (!buffer.WriteBigEndian16(sizeof(supportedSignatureAndHashAlgorithms) + sizeof(uint16_t))) return false;
        if (!buffer.WriteBigEndian16(sizeof(supportedSignatureAndHashAlgorithms))) return false;
        if (!buffer.WriteBuffer(supportedSignatureAndHashAlgorithms, sizeof(supportedSignatureAndHashAlgorithms))) return false;
    }
    if (renegotiationInfoExtension) {
        if (!buffer.WriteBigEndian16((uint16_t)TLSExtension::RenegotiationInfo)) return false;
        if (!buffer.WriteBigEndian16(1)) return false;
        if (!buffer.Write<uint8_t>(0x00)) return false;
    }
    if (hostSize) {
        if (!buffer.WriteBigEndian16((uint16_t)TLSExtension::ServerName)) return false;
        if (!buffer.WriteBigEndian16(sizeof(uint16_t) + sizeof(TLSServerNameType) + sizeof(uint16_t) + hostSize)) return false;
        if (!buffer.WriteBigEndian16(sizeof(TLSServerNameType) + sizeof(uint16_t) + hostSize)) return false;
        if (!buffer.Write<TLSServerNameType>(TLSServerNameType::Host)) return false;
        if (!buffer.WriteBigEndian16(hostSize)) return false;
        if (!buffer.WriteCollection<char>(host)) return false;
    }
    return WriteCollection<uint8_t>(buffer);
}
size_t TLS::ReadSizedBuffer(void* buffer, size_t size) {
    if (!size) return 0;
    const size_t dataBufferSize = dataBuffer.GetSize();
    uint8_t* const buffer8 = (uint8_t*)buffer;
    if (!dataBufferSize) {
        TLSRecordHeader record;
        while (true) {
            if (!base.Read<TLSRecordHeader>(record)) return 0;
            record.size = MathLib::SwapEndian16(record.size);
            if (record.type != TLSRecordHeader::Type::Alert) break;
            TLSAlert alert;
            if (!base.Read<TLSAlert>(alert) || alert.level == TLSAlert::Level::Fatal || !base.Skip(record.size - sizeof(TLSAlert))) return 0;
        }
        if (record.type != mode) return 0;
        // TODO: Decrypt
        dataBuffer = record.size;
        if (!base.ReadCollection<uint8_t>(dataBuffer)) return 0;
        return ReadSizedBuffer(buffer, size);
    }
    if (dataBufferSize <= size) {
        for (size_t i = 0; i < dataBufferSize; i++) buffer8[i] = dataBuffer.AtUnsafe(i);
        dataBuffer = MathLib::Array<uint8_t>();
        return dataBufferSize + ReadSizedBuffer(buffer8 + dataBufferSize, size - dataBufferSize);
    }
    for (size_t i = 0; i < size; i++) buffer8[i] = dataBuffer.AtUnsafe(i);
    MathLib::Array<uint8_t> tmp = dataBufferSize - size;
    for (size_t i = size; i < dataBufferSize; i++) tmp.AtUnsafe(i - size) = dataBuffer.AtUnsafe(i);
    dataBuffer = tmp;
    return size;
}
size_t TLS::WriteSizedBuffer(const void* buffer, size_t size) {
    const uint16_t limit = 1 << 14;
    const uint8_t* const buffer8 = (const uint8_t*)buffer;
    size_t position = 0;
    while (size) {
        const uint16_t curr = size <= limit ? size : limit;
        if (!base.Write<TLSRecordHeader>(TLSRecordHeader(mode, MathLib::SwapEndian16(version), MathLib::SwapEndian16(curr)))) break;
        // TODO: Encrypt
        if (!base.WriteBuffer(buffer8 + position, curr)) break;
        position += curr;
        size -= curr;
    }
    return position;
}
bool TLS::Skip(size_t size) {
    return DefaultSkip(size);
}