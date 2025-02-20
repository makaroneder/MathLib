#ifndef MathLib_Emulator_Step_H
#define MathLib_Emulator_Step_H
#include "Emulator.hpp"
#include "Register.hpp"

namespace MathLib {
    struct StepEmulator : Emulator {
        StepEmulator(const Array<uint8_t>& memory);
        [[nodiscard]] virtual bool Run(void) override;
        [[nodiscard]] virtual bool Step(void) = 0;
        [[nodiscard]] virtual Register GetPC(void) const = 0;
    };
}

#endif