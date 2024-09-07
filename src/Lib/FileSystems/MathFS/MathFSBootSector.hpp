#ifndef MathFSBootSector_H
#define MathFSBootSector_H
#include <stdint.h>

struct MathFSBootSector {
    static constexpr const uint8_t expectedSignature1 = 'M';
    static constexpr const uint16_t expectedSignature2 = 0xaa55;
    uint8_t code[501];
    uint8_t signature1;
    uint64_t root;
    uint16_t signature2;

    MathFSBootSector(void);
    MathFSBootSector(uint64_t root);
    bool IsValid(void) const;
} __attribute__((packed));

#endif