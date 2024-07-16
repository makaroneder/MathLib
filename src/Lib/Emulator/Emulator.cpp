#include "Emulator.hpp"

Emulator::Emulator(const Array<uint8_t>& mem) : memory(mem) {}
size_t Emulator::GetSize(void) const {
    return memory.GetSize();
}