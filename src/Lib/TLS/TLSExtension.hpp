#ifndef TLS_Extension_H
#define TLS_Extension_H
#include <stdint.h>

namespace MathLib {
    enum class TLSExtension : uint16_t {
        ServerName = 0x0000,
        MaxFragmentLength,
        ClientCertificateURL,
        TrustedCAKeys,
        TruncatedHMAC,
        StatusRequest,
        SignatureAlgorithms = 0x000d,
        Padding = 0x0015,
        EncryptThenMAC,
        RenegotiationInfo = 0xff01,
    };
}

#endif