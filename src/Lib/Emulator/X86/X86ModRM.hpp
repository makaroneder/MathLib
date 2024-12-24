#ifndef MathLib_Emulator_X86_ModRM_H
#define MathLib_Emulator_X86_ModRM_H
#include <stdint.h>

namespace MathLib {
    union X86ModRM {
        enum class AddressingMode {
            BPlusSI = 0,
            BPlusDI,
            BPPlusSI,
            BPPlusDI,
            SI,
            DI,
            BP,
            ConstAddress = BP,
            BX,
        };
        enum class DisplacementMode {
            NoDisplacement,
            Bit8,
            Bit16,
            Register,
        };
        struct {
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;
        };
        uint8_t value;

        X86ModRM(uint8_t val = 0);
    };
}

#endif