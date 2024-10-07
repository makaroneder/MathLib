#include "Emulator.hpp"

Emulator::Emulator(const Array<uint8_t>& mem) : memory(mem) {}
size_t Emulator::GetSize(void) const {
    return memory.GetSize();
}
size_t Emulator::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
    size_t s = size;
    if (position + size > GetSize()) s = GetSize() - position;
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < s; i++) buff8[i] = memory.At(position + i);
    return s;
}
size_t Emulator::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
    size_t s = size;
    if (position + size > GetSize()) s = GetSize() - position;
    const uint8_t* buff8 = (const uint8_t*)buffer;
    for (size_t i = 0; i < s; i++) memory.At(position + i) = buff8[i];
    return s;
}