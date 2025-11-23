#ifdef __x86_64__
#ifndef FISRegH2D_H
#define FISRegH2D_H
#include <stdint.h>

enum class FISType : uint8_t {
    RegH2D = 0x27,
};
struct FISRegH2D {
    FISType type;
    uint8_t portMultiplier : 4;
    uint8_t reserved1 : 3;
    uint8_t commandControl : 1;
    uint8_t command;
    uint8_t featureLow;
    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t deviceReg;
    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t featureHigh;
    uint16_t count;
    uint8_t isoCommandCompletion;
    uint8_t control;
    uint32_t reserved2;
};

#endif
#endif