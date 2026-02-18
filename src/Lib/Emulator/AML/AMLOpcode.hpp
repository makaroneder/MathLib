#ifndef MathLib_Emulator_AML_Opcode_H
#define MathLib_Emulator_AML_Opcode_H
#include <stdint.h>

namespace MathLib {
    enum class AMLOpcode : uint16_t {
        NullName = 0x00,
        Zero = NullName,
        One,
        Name = 0x08,
        Byte = 0x0a,
        Word,
        DWord,
        String,
        QWord,
        Scope = 0x10,
        Buffer,
        Package,
        DualNamePrefix = 0x2e,
        MultiNamePrefix,
        Extension = 0x5b,
        Ones = 0xff,
        OperationRegion = 0x5b80,
        Field,
        Device,
    };
}

#endif