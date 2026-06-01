#include "SequenceReadable.hpp"

namespace MathLib {
    SequenceReadable::SequenceReadable(const Sequence<uint8_t>& sequence) : sequence(sequence), position(0) {}
    void SequenceReadable::ResetPosition(void) {
        position = 0;
    }
    size_t SequenceReadable::ReadSizedBuffer(void* buffer, size_t size) {
        uint8_t* const buffer8 = (uint8_t*)buffer;
        size = Min<size_t>(size, sequence.GetSize() - position);
        for (size_t i = 0; i < size; i++) buffer8[i] = sequence.AtUnsafe(i + position);
        position += size;
        return size;
    }
    bool SequenceReadable::Skip(size_t size) {
        if (sequence.GetSize() < position + size) return false;
        position += size;
        return true;
    }
}