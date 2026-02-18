#include "StepEmulator.hpp"

namespace MathLib {
    StepEmulator::StepEmulator(const Sequence<uint8_t>& memory) : Emulator(memory) {}
    bool StepEmulator::Run(void) {
        while (GetPC().value < GetSize())
            if (!Step()) return false;
        return true;
    }
}