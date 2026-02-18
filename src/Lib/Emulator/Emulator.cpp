#include "Emulator.hpp"

namespace MathLib {
    Emulator::Emulator(void) {}
    Emulator::Emulator(const Sequence<uint8_t>& memory) : memory(CollectionToArray<uint8_t>(memory)) {}
    size_t Emulator::GetSize(void) const {
        return memory.GetSize();
    }
    size_t Emulator::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        const size_t s = position + size > GetSize() ? GetSize() - position : size;
        uint8_t* buff8 = (uint8_t*)buffer;
        for (size_t i = 0; i < s; i++) buff8[i] = memory.At(position + i);
        return s;
    }
    size_t Emulator::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        const size_t s = position + size > GetSize() ? GetSize() - position : size;
        const uint8_t* buff8 = (const uint8_t*)buffer;
        for (size_t i = 0; i < s; i++) memory.At(position + i) = buff8[i];
        return s;
    }
}