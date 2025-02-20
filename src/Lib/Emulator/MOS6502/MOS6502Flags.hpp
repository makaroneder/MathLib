#ifndef MathLib_Emulator_MOS6502_Flags_H
#define MathLib_Emulator_MOS6502_Flags_H
#include <stdint.h>

namespace MathLib {
    union MOS6502Flags {
        uint8_t value;
        struct {
            bool carry : 1;
            bool zero : 1;
            bool interruptDisable : 1;
            bool decimal : 1;
            bool breakFlag : 1;
            bool reserved : 1;
            bool overflow : 1;
            bool negative : 1;
        } __attribute__((packed));
        MOS6502Flags(uint8_t value = 0);
    } __attribute__((packed));
}

#endif