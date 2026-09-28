#ifndef TLS_MaxFragmentLength_H
#define TLS_MaxFragmentLength_H
#include <stdint.h>

namespace MathLib {
    enum class TLSMaxFragmentLength : uint8_t {
        To9 = 0x01,
        To10,
        To11,
        To12,
    };
}

#endif