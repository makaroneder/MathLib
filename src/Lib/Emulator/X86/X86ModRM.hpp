#ifndef Emulator_X86_ModRM_H
#define Emulator_X86_ModRM_H
#include <stdint.h>

union X86ModRM {
    struct {
        uint8_t dst : 3;
        uint8_t src : 3;
        uint8_t mode : 2;
    };
    uint8_t value;

    X86ModRM(uint8_t val = 0);
};

#endif