#ifndef Int24_H
#define Int24_H
#include <stdint.h>

struct Int24 {
    uint8_t data[3];

    Int24(void);
    Int24(uint32_t value);
    [[nodiscard]] uint32_t Get(void) const;
    [[nodiscard]] Int24 SwapEndian(void) const;
} __attribute__((packed));

#endif