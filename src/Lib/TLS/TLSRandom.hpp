#ifndef TLS_Random_H
#define TLS_Random_H
#include <stdint.h>

namespace MathLib {
    struct TLSRandom {
        uint8_t random[32];

        [[nodiscard]] bool operator==(const TLSRandom& other) const;
        [[nodiscard]] bool operator!=(const TLSRandom& other) const;
    } __attribute__((packed));
}

#endif