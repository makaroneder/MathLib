#ifndef TLS_ServerNameType_H
#define TLS_ServerNameType_H
#include <stdint.h>

namespace MathLib {
    enum class TLSServerNameType : uint8_t {
        Host = 0x00,
    };
}

#endif