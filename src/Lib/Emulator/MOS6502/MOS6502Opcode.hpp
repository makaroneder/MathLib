#ifndef MathLib_Emulator_MOS6502_Opcode_H
#define MathLib_Emulator_MOS6502_Opcode_H
#include <stdint.h>

namespace MathLib {
    enum class MOS6502Opcode : uint8_t {
        Break = 0x00, // TODO:
        OrAIndirectX,
        OrAZeroPage = 0x05,
        ArithmeticShiftLeftZeroPage,
        PushFlags = 0x08, // TODO:
        OrAImmediate,
        ArithmeticShiftLeftImplied,
        OrAAbsolute = 0x0d,
        ArithmeticShiftLeftAbsolute,
        BranchOnPlus = 0x10, // TODO:
        OrAIndirectY,
        OrAZeroPageX = 0x15,
        ArithmeticShiftLeftZeroPageX,
        ClearCarry = 0x18,
        OrAAbsoluteY,
        OrAAbsoluteX = 0x1d,
        ArithmeticShiftLeftAbsoluteX,
        // TODO: 0x20+
    };
}

#endif