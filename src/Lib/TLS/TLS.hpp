#ifndef TLS_H
#define TLS_H
#include "TLSRandom.hpp"
#include "TLSRecordHeader.hpp"
#include "EncryptionMethod.hpp"
#include "../Interfaces/RWDevice.hpp"
#include "TLSSignatureAndHashAlgorithm.hpp"

namespace MathLib {
    struct TLS : RWDevice {
        static constexpr uint16_t version = 0x0303;
        static constexpr TLSCipherSuite supportedCipherSuites[] = {
            TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA256,
            TLSCipherSuite::DHE_RSA_AES_256_CBC_SHA,
            TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA256,
            TLSCipherSuite::DHE_RSA_AES_128_CBC_SHA,
        };
        static constexpr TLSCompressionMethod supportedCompressionMethods[] = {
            TLSCompressionMethod::None,
        };
        static TLSSignatureAndHashAlgorithm supportedSignatureAndHashAlgorithms[];

        TLS(RWDevice& base);
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;
        [[nodiscard]] bool PerformHandshake(const Collection<char>& host);

        private:
        Array<uint8_t> dataBuffer;
        RWDevice& base;
        EncryptionMethod writeEncryption;
        EncryptionMethod readEncryption;
        uint64_t writeSequenceNumber;
        uint64_t readSequenceNumber;
        TLSRecordHeader::Type mode;
    };
}

#endif