#include "Emulator.hpp"

namespace MathLib {
    Emulator::Emulator(const Array<uint8_t>& memory) : memory(memory) {
        EmptyBenchmark
    }
    size_t Emulator::GetSize(void) const {
        StartBenchmark
        ReturnFromBenchmark(memory.GetSize());
    }
    size_t Emulator::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        StartBenchmark
        const size_t s = position + size > GetSize() ? GetSize() - position : size;
        uint8_t* buff8 = (uint8_t*)buffer;
        for (size_t i = 0; i < s; i++) buff8[i] = memory.At(position + i);
        ReturnFromBenchmark(s);
    }
    size_t Emulator::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        StartBenchmark
        const size_t s = position + size > GetSize() ? GetSize() - position : size;
        const uint8_t* buff8 = (const uint8_t*)buffer;
        for (size_t i = 0; i < s; i++) memory.At(position + i) = buff8[i];
        ReturnFromBenchmark(s);
    }
}