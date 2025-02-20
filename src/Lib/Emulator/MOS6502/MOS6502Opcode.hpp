#ifndef MathLib_Emulator_MOS6502_Opcode_H
#define MathLib_Emulator_MOS6502_Opcode_H
#include <stdint.h>

namespace MathLib {
    enum class MOS6502Opcode : uint8_t {
        Brk = 0x00,
        OrAIndirectX,
        OrAZeroPage = 0x05,
        ASLZeroPage,
        PHP = 0x08,
        OrAImmediate,
    };
}

#endif