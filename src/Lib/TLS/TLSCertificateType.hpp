#ifndef TLS_CertificateType_H
#define TLS_CertificateType_H
#include <stdint.h>

namespace MathLib {
    enum class TLSCertificateType : uint8_t {
        RSASign = 1,
        DSSSign,
        RSAFixedDH,
        DSSFixedDH,
        RSAEphemeralDH,
        DSSEphemeralDH,
        FortezzaDMS = 20,
    };
}

#endif