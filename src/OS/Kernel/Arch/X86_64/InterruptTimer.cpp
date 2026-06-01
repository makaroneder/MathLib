#ifdef __x86_64__
#include "InterruptTimer.hpp"

void InterruptTimer::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    time += 1 / GetFrequency();
}

#endif