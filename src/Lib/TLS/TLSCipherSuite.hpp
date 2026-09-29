#ifndef TLS_CipherSuite_H
#define TLS_CipherSuite_H
#include <stdint.h>

namespace MathLib {
    enum class TLSCipherSuite : uint16_t {
        None = 0x0000,
        DHE_DSS_AES_128_CBC_SHA = 0x0032,
        DHE_RSA_AES_128_CBC_SHA,
        DH_Anonymous_AES_128_CBC_SHA,
        DHE_DSS_AES_256_CBC_SHA = 0x0038,
        DHE_RSA_AES_256_CBC_SHA,
        DH_Anonymous_AES_256_CBC_SHA,
        DHE_DSS_AES_128_CBC_SHA256 = 0x0040,
        DHE_RSA_AES_128_CBC_SHA256 = 0x0067,
        DHE_DSS_AES_256_CBC_SHA256 = 0x006a,
        DHE_RSA_AES_256_CBC_SHA256,
        DH_Anonymous_AES_128_CBC_SHA256,
        DH_Anonymous_AES_256_CBC_SHA256,
    };
}

#endif