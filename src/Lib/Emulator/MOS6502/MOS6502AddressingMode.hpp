#ifndef MathLib_Emulator_MOS6502_AddressingMode_H
#define MathLib_Emulator_MOS6502_AddressingMode_H
#include <stdint.h>

namespace MathLib {
    enum class MOS6502AddressingMode : uint8_t {
        IndirectX = 0,
        ZeroPage,
        Immediate,
        Absolute,
        IndirectY,
        ZeroPageX,
        AbsoluteY,
        AbsoluteX,
    };
}

#endif