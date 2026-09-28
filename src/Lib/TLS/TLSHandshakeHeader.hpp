#ifndef TLS_HandshakeHeader_H
#define TLS_HandshakeHeader_H
#include "Int24.hpp"

namespace MathLib {
    struct TLSHandshakeHeader {
        enum class Type : uint8_t {
            HelloRequest = 0x00,
            ClientHello,
            ServerHello,
            Certificate = 0x0b,
            ServerKeyExchange,
            CertificateRequest,
            ServerHelloDone,
            CertificateVerify,
            ClientKeyExchange,
            Finished = 0x14,
            CertificateURL,
            CertificateStatus,
        };
        Type type;
        Int24 size;

        TLSHandshakeHeader(void);
        TLSHandshakeHeader(Type type, Int24 size);
    } __attribute__((packed));
}

#endif