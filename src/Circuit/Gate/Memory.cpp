#include "Memory.hpp"

Memory::Memory(size_t size) : memory(size) {}
Memory::Memory(const MathLib::Sequence<bool>& memory) : memory(memory) {}
MathLib::Bitmap Memory::Update(const MathLib::Sequence<bool>& input) {
    const size_t size = input.GetSize();
    if (size < 2) return MathLib::Bitmap();
    size_t address = 0;
    for (size_t i = 0; i < size - 2; i++) address |= input.At(i) << i;
    if (address >= memory.GetSize()) return MathLib::Bitmap();
    if (input.At(size - 2))
        if (!memory.Set(address, input.At(size - 1))) return MathLib::Bitmap();
    MathLib::Bitmap ret = MathLib::Bitmap(1);
    ret.Fill(memory.At(address));
    return ret.AddSequence(memory) ? ret : MathLib::Bitmap();
}