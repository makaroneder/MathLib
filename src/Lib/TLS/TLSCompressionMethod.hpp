#ifndef TLS_CompressionMethod_H
#define TLS_CompressionMethod_H
#include <stdint.h>

namespace MathLib {
    enum class TLSCompressionMethod : uint8_t {
        None = 0x00,
    };
}

#endif