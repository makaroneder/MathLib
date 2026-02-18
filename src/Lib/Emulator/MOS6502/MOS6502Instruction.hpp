#ifndef MathLib_Emulator_MOS6502_Instruction_H
#define MathLib_Emulator_MOS6502_Instruction_H
#include <stdint.h>

namespace MathLib {
    union MOS6502Instruction {
        uint8_t value;
        struct {
            uint8_t opcode1 : 2;
            uint8_t addressingMode : 3;
            uint8_t opcode2 : 3;
        };

        [[nodiscard]] bool operator==(const MOS6502Instruction& other) const;
        [[nodiscard]] bool operator!=(const MOS6502Instruction& other) const;
    } __attribute__((packed));
}

#endif