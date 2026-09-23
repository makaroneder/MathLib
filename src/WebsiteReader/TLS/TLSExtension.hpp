#ifndef TLSExtension_H
#define TLSExtension_H
#include <stdint.h>

enum class TLSExtension : uint16_t {
    ServerName = 0x0000,
    SignatureAlgorithms = 0x000d,
    Padding = 0x0015,
    RenegotiationInfo = 0xff01,
};

#endif