#ifndef TLSCipherSuite_H
#define TLSCipherSuite_H
#include <stdint.h>

enum class TLSCipherSuite : uint16_t {
    None = 0x0000,
    DHE_RSA_AES_128_CBC_SHA = 0x0033,
    DHE_RSA_AES_256_CBC_SHA = 0x0039,
    DHE_RSA_AES_128_CBC_SHA256 = 0x0067,
    DHE_RSA_AES_256_CBC_SHA256 = 0x006b,
};

#endif