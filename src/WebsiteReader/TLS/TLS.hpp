#ifndef TLS_H
#define TLS_H
#include "TLSRandom.hpp"
#include "TLSCipherSuite.hpp"
#include "TLSRecordHeader.hpp"
#include "TLSCompressionMethod.hpp"
#include "TLSSignatureAndHashAlgorithm.hpp"
#include <Interfaces/RWDevice.hpp>

struct TLS : MathLib::RWDevice {
    static constexpr uint16_t version = 0x0303;
    static constexpr TLSCipherSuite supportedCipherSuites[] = {
        // TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256,
        // TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA,
        // TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256,
        TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA,
    };
    static constexpr TLSCompressionMethod supportedCompressionMethods[] = {
        TLSCompressionMethod::None,
    };
    static TLSSignatureAndHashAlgorithm supportedSignatureAndHashAlgorithms[];

    TLS(MathLib::RWDevice& base);
    [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
    [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
    [[nodiscard]] virtual bool Skip(size_t size) override;
    [[nodiscard]] bool PerformHandshake(const MathLib::Collection<char>& host);

    private:
    [[nodiscard]] bool SendClientHello(const MathLib::Collection<char>& host, const TLSRandom& random);

    MathLib::Array<uint8_t> dataBuffer;
    MathLib::RWDevice& base;
    TLSCipherSuite cipherSuite;
    TLSCompressionMethod compressionMethod;
    TLSRecordHeader::Type mode;
};

#endif