#include "StepEmulator.hpp"

namespace MathLib {
    StepEmulator::StepEmulator(const Array<uint8_t>& memory) : Emulator(memory) {
        EmptyBenchmark
    }
    bool StepEmulator::Run(void) {
        StartBenchmark
        while (GetPC().value < GetSize())
            if (!Step()) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
}