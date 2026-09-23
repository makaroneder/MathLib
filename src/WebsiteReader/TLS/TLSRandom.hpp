#ifndef TLSRandom_H
#define TLSRandom_H
#include <stdint.h>

struct TLSRandom {
    uint8_t random[32];

    [[nodiscard]] bool operator==(const TLSRandom& other) const;
    [[nodiscard]] bool operator!=(const TLSRandom& other) const;
} __attribute__((packed));

#endif