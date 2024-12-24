#ifndef MathLib_Emulator_MathArch_Opcode_H
#define MathLib_Emulator_MathArch_Opcode_H
#include <stdint.h>

namespace MathLib {
    enum class MathArchOpcode : uint8_t {
        Nop = 0x00,
        SetPCToConst,
        SetSPToConst,
        PushConst,
        PushA,
        PopToA,
        Extended = 0xff,
    };
}

#endif